#include "MeshModel.h"
#include <unordered_map>
#include <vector>
#include <utility>


//constructor
MeshModel::MeshModel(Wavefront_obj obj)
{
	objFaces.reserve(10000);
	objPoints.reserve(10000);
	
	this->loadObj(obj);//get object from obj file
	this->BB.calcBBox(objPoints);//calculate bounding box
	this->cannonizeObj();//normelize object and update bounding box
	comupteTB();
}

MeshModel::MeshModel() {}//maybe delete





//load object from waveFront structure
void MeshModel::loadObj(Wavefront_obj obj)
{
	//load objPoints
	for (int i = 0; i < obj.m_points.size(); i++)
	{
		this->objPoints.emplace_back(obj.m_points[i][0],
			obj.m_points[i][1], obj.m_points[i][2],1);
	}
	//load objFaces and objSegs
	for (int i = 0; i < obj.m_faces.size(); i++)
	{
		int a, b, c;
		a = obj.m_faces[i].v[0];
		b = obj.m_faces[i].v[1];
		c = obj.m_faces[i].v[2];

		//sort point indexes
		objFaces.emplace_back(a, b, c);
		if (a > c)
			std::swap(a, c);
		if (a > b)
			std::swap(a, b);
		if (b > c)
			std::swap(b, c);

	}
	objNormals.resize(obj.m_points.size());
	comupteNormals();
	//load objFaces and objSegs

	std::unordered_map<std::pair<int, int>, int, pair_hash> index_map;
	objFaces.clear();
	objPoints.clear();
	objFaces.resize(obj.m_faces.size()); // Resize to hold all faces
	
	for (int i = 0; i < obj.m_faces.size(); i++)
	{
		
		const auto& face = obj.m_faces[i];
		for (int j = 0; j < 3; j++) {
			auto key = std::make_pair(face.v[j], face.t[j]);
			auto it = index_map.find(key);
			if (it != index_map.end()) {
				objFaces[i][j] = it->second;
			}
			else {
				int index = objPointTex.size();
				objPointTex.emplace_back(key);
				index_map[key] = index;
				objFaces[i][j] = index;
			}
		}

	}
	int j = 0;
	objPoints.resize(objPointTex.size());
	objTextCords.resize(objPointTex.size());
	std::vector<glm::vec4> copy = objNormals;
	objNormals.resize(objPointTex.size());
	for (auto &pair: objPointTex)
	{
		objPoints.at(j) = { obj.m_points[pair.first][0],
			obj.m_points[pair.first][1], obj.m_points[pair.first][2],1 };
		objNormals.at(j) = copy.at(pair.first);
		if (obj.m_textureCoordinates.size()== 0)
		{
			objTextCords.at(j) = { 0.5,0.5 };
		}
		else
		{
			objTextCords.at(j) = { obj.m_textureCoordinates[pair.second][0],
			obj.m_textureCoordinates[pair.second][1] };
		}
			j++;
			
	}
	

}
void MeshModel::comupteNormals()
{
	
	for (int i = 0; i < objFaces.size(); i++)
	{
		//get two face edges
		glm::vec3 e1 = (objPoints.at(objFaces.at(i)[0]) -
			objPoints.at(objFaces.at(i)[1]));
		glm::vec3 e2 = objPoints.at(objFaces.at(i)[0]) -
			objPoints.at(objFaces.at(i)[2]);

		glm::vec3 normal = glm::cross(e1, e2);//compute face normal(un normalized)
		glm::vec4 normal4 = { normal,0 };//compute face normal(un normalized)
		//add weighted normal to relevent vertices
		objNormals[objFaces.at(i)[0]] += normal4;
		objNormals[objFaces.at(i)[1]] += normal4;
		objNormals[objFaces.at(i)[2]] += normal4;

	}
	//normalize
	for (auto& normal : objNormals) {
		normal.w = 0;
		normal = glm::normalize(normal);
	}
	
}
void MeshModel::comupteTB()
{
	objTangents.resize(objPoints.size());
	objBiTangents.resize(objPoints.size());
	for (int i = 0; i < objPoints.size(); i++)
	{
		objTangents.at(i) = { 0.0f,0.0f,0.0f };
		objBiTangents.at(i) = { 0.0f,0.0f,0.0f };
	}
	for (int i = 0; i < objFaces.size(); i++)
	{
		auto& face = objFaces.at(i);
		glm::vec3 e1 = objPoints.at(face[1]) - objPoints.at(face[0]);
		glm::vec3 e2 = objPoints.at(face[2]) - objPoints.at(face[0]);
		glm::vec2 dUV1 = objTextCords.at(face[1]) - objTextCords.at(face[0]);
		glm::vec2 dUV2 = objTextCords.at(face[2]) - objTextCords.at(face[0]);

		float f = 1.0f / (dUV1.x * dUV2.y - dUV2.x * dUV1.y);

		glm::vec3 T = f * (dUV2.y * e1 - dUV1.y * e2);
		glm::vec3 B = f * (-dUV2.x * e1 + dUV1.x * e2);
		
		for (int j = 0; j < 3; j++) {
			objTangents[face[j]] += T;
			objBiTangents[face[j]] += B;
		}

	}
	for (int i = 0; i < objPoints.size(); i++) {
		// 1. Normalize accumulated vectors
		objTangents[i] = glm::normalize(objTangents[i]);
		objBiTangents[i] = glm::normalize(objBiTangents[i]);



		// 2. Orthogonalize with Gram-Schmidt
		glm::vec3 N = objNormals[i];
		objTangents[i] = glm::normalize(objTangents[i] - glm::dot(objTangents[i], N) * N);

		// 3. Re-calculate bitangent to ensure orthogonality
		objBiTangents[i] = glm::cross(N, objTangents[i]);
	
	}


}
//normalise object size and location
void MeshModel::cannonizeObj()
{
	//calculate longest axis
	double lenX = glm::abs(BB.BB[1] - BB.BB[0]);
	double lenY = glm::abs(BB.BB[3] - BB.BB[2]);
	double lenZ = glm::abs(BB.BB[5] - BB.BB[4]);
	double scalingF = std::max(lenX,lenY);
	scalingF = std::max(scalingF, lenZ);
	//scale to 10 units
	scalingF = 10 / scalingF;

	//calculate location translations
	double transX = -(BB.BB[1] - (lenX / 2));
	double transY = -(BB.BB[3] - (lenY / 2));
	double transZ = -(BB.BB[5] - (lenZ / 2));



	//use matrices to preform the translation and scale
	glm::mat4x4 transMat = getTranslation({ float(transX),float(transY),float(transZ) });
	glm::mat4x4 scaleMat = getScale({ float(scalingF),float(scalingF),float(scalingF) });
	glm::mat4x4 cannoniseMat = transMat * scaleMat;//calculate full matrix
	//apply on all points
	for (int i = 0; i < objPoints.size(); i++)
	{
		objPoints.at(i) = objPoints.at(i)* cannoniseMat;
	}

	//calculate new bounding box points;
	BB.calcBBox(objPoints);

}

