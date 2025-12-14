#pragma once
#include <glm/glm.hpp>
#include "UtilFuncs.h"
//class to simulate the camera
class Camera
{
public:
	//constructor
	Camera();
	glm::mat4x4 prespMat;//matrix to create a cube from a frustrom in space
	glm::mat4x4 viewMat;//matrix to match cube coords to screen coords
	glm::mat4x4 LaMat;//matrix to make the camera look at the object
	glm::vec3 position;//positions of the camera
	float aspect_ratio;
	void setPrespMat(float n, float f, float r, float t);//set presMat with right and top
	void setPrespMat(float n, float f, float FoV);//set prespMat with Fov
	void setViewMat(float width, float height);//set viewPortMat
	void moveCam(float x, float y, float z);// move camera position
	void lookAt(float x, float y, float z);//calculate lookat matrix
	float f;
	float n;
	//getters
	glm::mat4x4 getPrespMat();
	glm::mat4x4 getViewMat();
};

