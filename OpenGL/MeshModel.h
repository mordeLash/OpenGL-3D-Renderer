#pragma once
#include <set>
#include <Utils/wavefront_obj.h>
#include "UtilFuncs.h"
#include <glm/glm.hpp>
#include "BBox.h"
//class to represent the geometry of a object
class MeshModel
{
public:
	//constructor
	MeshModel(Wavefront_obj obj);
	MeshModel();
	std::vector < glm::vec4> objPoints; //edges
	std::vector < glm::uvec3> objFaces;	//triagle faces
	std::vector < glm::vec4> objNormals;//object verticy normals 
	std::vector <glm::vec3> objTangents;
	std::vector <glm::vec3> objBiTangents;
	std::vector <glm::vec2> objTextCords; //texture coordinants
	std::vector <std::pair<int, int>> objPointTex;	//triagle faces
	glm::vec3 objMaterial;//k_ambient,k_diffuse,k_specular
	bool single_sided; //is object single sided
	BBox BB;//bounding box

protected:
	void comupteNormals();//calculate normals per vertex
	void comupteTB();//calculate normals per vertex
	void cannonizeObj(); // normalise the objects size and position
	void loadObj(Wavefront_obj obj);//get object from .obj file
};

//struct to maintain list of unique pairs of v and vt
struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2>& p) const {
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);
		return h1 ^ (h2 << 1);
	}
};