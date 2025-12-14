#include "Light.h"

//constructors
Light::Light(int type, double pos_dir[3], float intensity[3])
{
	this->type = type;
	this->pos_dir[0] = pos_dir[0];
	this->pos_dir[1] = pos_dir[1];
	this->pos_dir[2] = pos_dir[2];
	this->intensity[0] = intensity[0];
	this->intensity[1] = intensity[1];
	this->intensity[2] = intensity[2];
}

Light::Light()
{
	this->type = 4;
	this->pos_dir[0] = 0;
	this->pos_dir[1] = 0;
	this->pos_dir[2] = 0;
	this->intensity[0] = 1.0f;
	this->intensity[1] = 1.0f;
	this->intensity[2] = 1.0f;
}

Light::Light(int type, float intensity[3])
{
	this->type = type;
	this->intensity[0] = intensity[0];
	this->intensity[1] = intensity[1];
	this->intensity[2] = intensity[2];
}
