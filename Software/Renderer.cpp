#include "Renderer.h"

#include <iostream>
#include <assert.h>

extern GLuint g_programID;


Renderer::Renderer()
{
}


Renderer::Renderer(const unsigned int width, const unsigned int height) : 
	colorBuffer(width * height, 0), //allocate memory for the software color buffer and set all colors to black
	textureID(0) //texture id of 0 is reserved by OpenGL and is never a valid object
{
	this->width = width;
	this->height = height;
}


Renderer::~Renderer()
{
	if (glIsTexture(textureID))
	{
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
}


void Renderer::clearBufferToZero()
{
	std::memset(colorBuffer.data(), 0, colorBuffer.size() * sizeof(unsigned int));  // Set all elements to 0
}


//resizes and clear the software color buffer
void Renderer::resizeBuffer(const unsigned int width, const unsigned int height)
{
	this->width = width;
	this->height = height;
	colorBuffer.resize(width * height, 0);
}


//creates new texture. This texture is later on used to transfer the software buffer (colorBuffer) to the screen
void Renderer::createTexture(const unsigned int width, const unsigned int height)
{
	//std::cout << "Texture id: " << textureID << std::endl;

	if (glIsTexture(textureID))
	{
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	//std::cout << "Texture id: " << textureID << std::endl;

	// Assuming framebuffer contains pixel data in GL_RGB format
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Set texture filtering and wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Renderer::addPixel(unsigned int x, unsigned int y, BYTE r, BYTE g, BYTE b, BYTE a)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
	{
		unsigned int index = y * width + x;
		unsigned int color = (a << 24) | (r << 16) | (g << 8) | b;
		colorBuffer[index] = color;
	}
}


//this function turns on the specified pixels on screen by copying the data from the software color buffer (colorBuffer) to the real hardware color buffer
void Renderer::drawPixels()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	//std::cout << "Texture id: " << textureID << std::endl;

	//the texture is created in createTexture to avoid creation and deletion every frame. This only transfered the data to the texture.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, colorBuffer.data());

	GLuint fboID;
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);
	//std::cout << "Framebuffer id: " << fboID << std::endl;


	// Attach the texture to the FBO as the color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureID, 0);

	// Check if the FBO is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Error: Incomplete framebuffer!" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind the FBO

	// Bind the FBO for reading
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboID);

	// Bind the default framebuffer for drawing (0 is the default framebuffer)
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Blit the framebuffer contents to the screen
	glBlitFramebuffer(
		0, 0, width, height,    // Source rectangle (FBO)
		0, 0, width, height, // Destination rectangle (screen)
		GL_COLOR_BUFFER_BIT,    // What to copy
		GL_NEAREST              // Interpolation (GL_NEAREST avoids filtering)
	);

	// Unbind the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fboID);
}


