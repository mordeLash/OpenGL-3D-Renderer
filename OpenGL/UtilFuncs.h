#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <array>

struct Pixel
{
	int  x, y; //pixel in screen coordinates
	unsigned int color; //RGBA color format - 4 components of 8 bits each - 0xAABBGGRR - AA alpha, BB blue, RR red
};

struct PixInTri
{
	int  x, y; //pixel in screen coordinates
	double size, l1, l2, l3;//barycentric coords
};

glm::mat4x4 getTranslation(std::vector<float> trans); //translation trans
glm::mat4x4 getScale(std::vector<float> trans);//scale trans
glm::mat4x4 getRotation(float angle, int axis);//rotation trans
glm::vec3 computeTrans(glm::vec3 vec, glm::mat4x4 mat, bool normal = false); //compute transformation on a 3D point
glm::vec4 toCartesian(glm::vec4 vec);
glm::vec4 quatSLERP(glm::vec4 q1, glm::vec4 q2,float t);
glm::vec4 quatMult(glm::vec4 q1, glm::vec4 q2);
int roundF(float num);//round to nearest integer
