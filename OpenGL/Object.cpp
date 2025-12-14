#include "Object.h"
//constructor
Object::Object(Wavefront_obj obj)
{

	this->geometry = MeshModel(obj);//init geometry
	this->modelingMat = glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f, //init modeling mat
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	this->viewMat = glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f, //init viewMat
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	position = { 0,0,-15 };//init object position
	this->lights.resize(3);
	cam = Camera();//init camera
	cam.setViewMat(960, 640);

	alignWithView();//align object with starup camera


}
//maybe delete
Object::Object() {}

//change modeling matrix to allign with object initial position
void Object::alignWithView()
{
	viewMat = viewMat * getTranslation({ position.x, position.y, position.z });
}

void Object::calcModelViewMat()
{
	worldMat = modelingMat * viewMat;
}

void Object::calcPos()
{
	position = computeTrans({ 0,0,0 }, worldMat);//calcuate object position
}

