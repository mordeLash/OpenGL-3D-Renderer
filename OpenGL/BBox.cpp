#include "BBox.h"

//constructor
BBox::BBox()
{
	BB.resize(6);//six maxes
	BBoxPoints.resize(12);//twelve segments
	BoxSegs = { { 0, 1 }, { 0, 2 },{0, 4},
		{1, 3}, {1, 5} ,{2, 3} ,{2, 6} ,{3, 7},{4, 5},{4, 6},{5, 7},{6, 7} };//segments
}

void BBox::calcBBox(std::vector<glm::vec4> geomerty)
{
	BB = { geomerty.at(0).x,geomerty.at(0).x,
	geomerty.at(0).y, geomerty.at(0).y,
	geomerty.at(0).z, geomerty.at(0).z, };//LRDUNF
	for (int i = 1; i < geomerty.size(); i++)
	{
		glm::vec3 temp = geomerty.at(i);
		if (temp.x < BB[0])
		{
			BB[0] = temp.x;
		}
		else if (temp.x > BB[1])
		{
			BB[1] = temp.x;
		}
		if (temp.y < BB[2])
		{
			BB[2] = temp.y;
		}
		else if (temp.y > BB[3])
		{
			BB[3] = temp.y;
		}
		if (temp.z < BB[4])
		{
			BB[4] = temp.z;
		}
		else if (temp.z > BB[5])
		{
			BB[5] = temp.z;
		}
	}
	calcBBoxPoints();
}

void BBox::calcBBoxPoints()
{
	BBoxPoints = { {BB[0],BB[2],BB[4]},{BB[0],BB[3],BB[4]}, {BB[1],BB[2],BB[4]}, {BB[1],BB[3],BB[4]},
	{BB[0],BB[2],BB[5]},{BB[0],BB[3],BB[5]}, {BB[1],BB[2],BB[5]}, {BB[1],BB[3],BB[5]} };
}


