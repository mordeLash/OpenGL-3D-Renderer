#include "UtilFuncs.h"

glm::mat4x4 getTranslation(std::vector<float> trans)
{
	return glm::mat4x4(
		1.0f, 0.0f, 0.0f, trans[0],  // First column
		0.0f, 1.0f, 0.0f, trans[1],  // Second column
		0.0f, 0.0f, 1.0f, trans[2],  // Third column
		0.0f, 0.0f, 0.0f, 1.0f      // Fourth column
	);
}
//get scale trans mat
glm::mat4x4 getScale(std::vector<float> scale)
{
	glm::mat4x4 mat = glm::mat4x4(
		scale[0], 0.0f, 0.0f, 0.0f,  // First column
		0.0f, scale[1], 0.0f, 0.0f,  // Second column
		0.0f, 0.0f, scale[2], 0.0f,  // Third column
		0.0f, 0.0f, 0.0f, 1.0f      // Fourth column
	);
	return mat;
}
//get rotation trans mat
glm::mat4x4 getRotation(float angle, int axis)
{
	float rAngle = glm::radians(angle);
	float cosAngle = glm::cos(rAngle);
	float sinAngle = glm::sin(rAngle);
	glm::mat4x4 mat;
	switch (axis)
	{
	case 0:
		mat = glm::mat4x4(
			cosAngle, -sinAngle, 0.0f, 0.0f,  // First column
			sinAngle, cosAngle, 0.0f, 0.0f,  // Second column
			0.0f, 0.0f, 1.0f, 0.0f,  // Third column
			0.0f, 0.0f, 0.0f, 1.0f      // Fourth column
		);
		break;
	case 1:
		mat = glm::mat4x4(
			cosAngle, 0.0f, -sinAngle, 0.0f,  // First column
			0.0f, 1.0f, 0.0f, 0.0f,  // Second column
			sinAngle, 0.0f, cosAngle, 0.0f,  // Third column
			0.0f, 0.0f, 0.0f, 1.0f      // Fourth column
		);
		break;
	case 2:
		mat = glm::mat4x4(
			1.0f, 0.0f, 0.0f, 0.0f,  // First column
			0.0f, cosAngle, -sinAngle, 0.0f,  // Second column
			0.0f, sinAngle, cosAngle, 0.0f,  // Third column
			0.0f, 0.0f, 0.0f, 1.0f      // Fourth column
		);
		break;
	}
	return mat;
}

//util round function
int roundF(float num)
{
	return floor(num + 0.5);
}

//util function to comupte a transformation on a 3d point
glm::vec3 computeTrans(glm::vec3 vec, glm::mat4x4 mat, bool normal)
{
	glm::vec4 temp4 = { vec,1 }; //homogeneous coords
	if (normal)
	{
		temp4.w = 0;
	}
	temp4 = temp4 * mat;//compute transformation
	if (temp4.w == 0 || normal)
	{
		temp4.w = 1;
	}
	glm::vec3 temp3 = { temp4.x / temp4.w,temp4.y / temp4.w,temp4.z / temp4.w };//get euclidean coords
	return temp3;//return result
}

glm::vec4 toCartesian(glm::vec4 vec)
{
	if (vec.w == 0)
	{
		vec.w = 0.1;
	}
	return glm::vec4(vec.x / vec.w, vec.y / vec.w, vec.z / vec.w, vec.w);
}

glm::vec4 quatSLERP(glm::vec4 q1, glm::vec4 q2, float t)
{
	//q1(q1^c*q2)^t
	glm::vec4 result;
	glm::vec4 q1c = {q1[0],-q1[1],-q1[2],-q1[3]};
	glm::vec4 t1 = quatMult(glm::normalize(q1c), q2);
	glm::vec4 t2 = glm::pow(t1, { t,t,t,t });
	t2 = glm::normalize(t2);
	result = quatMult(q1,glm::normalize(t2));
	return result;
}

glm::vec4 quatMult(glm::vec4 q1, glm::vec4 q2)
{
	float s1 = q1[0];
	float s2 = q2[0];
	glm::vec3 v1 = { q1[1],q1[2],q1[3] };
	glm::vec3 v2 = { q2[1],q2[2],q2[3] };
	glm::vec3 vr;
	glm::vec4 result;
	result[0] = s1 * s2 - glm::dot(v1, v2);
	vr = s1 * v2+s2*v1+glm::cross(v1,v2);
	result[1] = vr.x;
	result[2] = vr.y;
	result[3] = vr.z;

	return glm::normalize(result);
}
