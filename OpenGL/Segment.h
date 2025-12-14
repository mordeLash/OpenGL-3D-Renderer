#pragma once
#include <vector>
//class to define a segment with a color
class Segment
{
public:

	Segment(int x1, int y1, int x2, int y2, unsigned int color);//constructor
	Segment();//constructor
	~Segment();//destructor
	//getters
	int getX1();
	int getX2();
	int getY1();
	int getY2();
	int getColor();
protected:
	//class variables
	int x1, y1, x2, y2;
	unsigned int color;

};

