#include "MeshModel.h"
#include "Camera.h"
#include "Segment.h"
#include "UtilFuncs.h"
#include <iostream>
#include <algorithm>
#include "Light.h"
#pragma once

//class to define an object in its entirty from file to screen(using other classes)
class Object
{
public:
	//constructor
	Object(Wavefront_obj obj);
	Object();

	MeshModel geometry;//objects geometry
	glm::mat4x4 modelingMat;//matrix for object space transformations
	glm::mat4x4 viewMat;//matrix for world space transformations
	glm::mat4x4 worldMat;//matrix for world space transformations
	Camera cam;//camera object
	glm::vec3 position;//objects position in world space
	unsigned int color;//object color
	bool double_sided;//object double sided
	int sHeight; //height of screen
	int sWidth; //height of screen
	float Ka;//object ambient coe
	float Kd;//object diffuse coe
	float Ks;//object specular coe
	int Sc;//object shiness coe
	void alignWithView();//allign object with startup camera
	std::vector<Light> lights;//scene lights
	void calcModelViewMat();//calculate world mat
	void calcPos();//calculate obj position in world

};

