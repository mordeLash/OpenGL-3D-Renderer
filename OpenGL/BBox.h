#pragma once
#include "Segment.h"
#include<glm/glm.hpp>
#include<vector>
//class to find and define bounding box of an objecy
class BBox
{
public:

	BBox();//constructor
	std::vector<float> BB; //the six maximums of the object
	std::vector<glm::vec3> BBoxPoints;//the points of the bounding box
	std::vector<glm::vec2> BoxSegs;// the pairs of points to draw the box
	void calcBBox(std::vector<glm::vec4> geomerty);//calculate the bounding box of given points
	void calcBBoxPoints();//init BBoxPoints

};

