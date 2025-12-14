#pragma once
#include <array> 
#include <vector> 
#include <glm/glm.hpp>

//class to calculate and sample a cubic bezier curve with 4 control
class Bezier
{
public:
	Bezier(std::array<glm::vec3,4> controlPoints);//consructor
	void sampleCurve(int numSegs);//sample the curve into curvePoints
	std::array<glm::vec3, 4> controlPoints; //control points for curve
	std::vector<glm::vec3> curvePoints; //sampled point curves
	glm::vec3 getPoint(float t);//get a point on the curve for value t
};

