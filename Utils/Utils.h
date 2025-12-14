#pragma once

#include <Glew/include/gl/glew.h>
#include "FreeImage/FreeImage.h"
#include <glm/glm.hpp>



//Macro to make code more readable
#define BUFFER_OFFSET(offset)   ((GLvoid*) (offset))
//strcut to define a screen pixel


struct Shader
{
	std::string filename;
	GLenum      type;
	std::string source;
};


void printMatrixArray(const glm::mat4x4& mat);
void antTeakBar_UIscale();
void printMatrix(const glm::mat4x4& mat);
void ConvertQuaternionToMatrix(const float quat[4], glm::mat4x4& rotationMatrix);
void createTranslationMatrix(float tx, float ty, float tz, glm::mat4x4& mat);
void createPerspectiveProjectionMatrix(float nearPlane, float farPlane, float right, float top, glm::mat4x4& mat);
void displayMessage(const std::string& str);
std::wstring getOpenFileName();
GLuint initShader(std::string vertexShaderFileName, std::string fragmentShaderFileName);
std::string readShaderSource(const std::string& shaderFileName);
void createCube(std::vector<glm::vec4>& positions, std::vector<glm::vec4>& colors, std::vector<glm::uvec3>& triangleIndices);
void displaySoftwareStatus();
bool LoadImage(const char* filename, BYTE** bits, unsigned int& width, unsigned int& height, FREE_IMAGE_COLOR_TYPE& colorType);
void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
void initGlew();
void debugOutputOpenGL();
