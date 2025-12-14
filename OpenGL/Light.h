#pragma once

//class to define a light in the scene
class Light
{
public:
	//constructors
	Light(int type, double pos_dir[3], float intensity[3]);
	Light();
	Light(int type, float intensity[3]);
	int type; //light type{ambient,direction,direction}
	double pos_dir[3]; //position or direction of the light
	float intensity[3]; //intesity of the light

};

