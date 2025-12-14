#pragma once

#include <vector>

#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>


class Renderer
{
public:
	Renderer(const unsigned int width, const unsigned int height);
	~Renderer();
	void clearBufferToZero();
	void resizeBuffer(const unsigned int width, const unsigned int height);
	void createTexture(const unsigned int width, const unsigned int height);
	void addPixel(unsigned int x, unsigned int y, BYTE r, BYTE g, BYTE b, BYTE a);
	void drawPixels();

private:
	Renderer(); //avoid using this constructor
	std::vector<unsigned int> colorBuffer;
	unsigned int width, height;
	GLuint textureID;
};



