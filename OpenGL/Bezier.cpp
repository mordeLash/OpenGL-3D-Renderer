#include "Bezier.h"

//consturtor
Bezier::Bezier(std::array<glm::vec3, 4> controlPoints)
{
	this->controlPoints = controlPoints;
}

void Bezier::sampleCurve(int numSegs)
{
	//sample curve using getPoint
	curvePoints.clear();
	curvePoints.resize(numSegs);
	float sample_len = 1.0f / (numSegs-1);
	float t = 0;
	for(int i = 0; i <= numSegs-1; i++)
	{
		curvePoints.at(i) = getPoint(t);
		t += sample_len;
	}
}

glm::vec3 Bezier::getPoint(float t)
{
	//get point on curve using bezier curve equation
	return
		(float(glm::pow(1 - t, 3)) * controlPoints[0] +
			3.0f * float(glm::pow(1 - t, 2)) * t * controlPoints[1] +
			3.0f * (1 - t) * float(glm::pow(t, 2)) * controlPoints[2] +
			float(glm::pow(t, 3)) * controlPoints[3]);
}
