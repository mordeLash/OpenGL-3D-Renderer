#include "Segment.h"


Segment::Segment(int x1, int y1, int x2, int y2, unsigned int color)
{
	this->color = color;
	if (x1 <= x2)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}
	else
	{
		this->x2 = x1;
		this->y2 = y1;
		this->x1 = x2;
		this->y1 = y2;
	}
}

Segment::Segment()
{
	x1 = 0;
	x2 = 0;
	y1 = 0;
	y2 = 0;
	color = 0xffffffff;
}

Segment::~Segment()
{
}

int Segment::getX1()
{
	return x1;
}
int Segment::getX2()
{
	return x2;
}
int Segment::getY1()
{
	return y1;
}
int Segment::getY2()
{
	return y2;
}

int Segment::getColor()
{
	return color;
}
