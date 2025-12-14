#include <windows.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "Utils.h"
#include <AntTweakBar/include/AntTweakBar.h>
#include <freeglut/include/GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>



void initGlew()
{
	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
		assert(0);
		exit(1);  // Exits the program with error status (1)
	}
}



void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	//the following is done to filter messages like this:
	//OpenGL Debug : Buffer detailed info : Buffer object 1 (bound to GL_ARRAY_BUFFER_ARB, usage hint is GL_STATIC_DRAW) will use VIDEO memory as the source for buffer object operations.
	if ((type == GL_DEBUG_TYPE_OTHER) && (id == 131185))
	{
		return;
	}

	std::cerr << "OpenGL Debug: " << message << std::endl;
}


void debugOutputOpenGL()
{
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(debugCallback, nullptr);
	//glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
#endif
}


void antTeakBar_UIscale()
{
	int screenWidth = glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight = glutGet(GLUT_SCREEN_HEIGHT);

	std::cout << "Screen resolution: " << screenWidth << "x" << screenHeight << std::endl;

	UINT dpi = GetDpiForSystem();
	std::cout << "System DPI: " << dpi << std::endl;

	//TwDefine(" GLOBAL fontscaling=1.0 "); // change the size of all UI elements in the AntTweakBar

	float UI_scalingFactor = screenWidth / 1920.0f;

	std::cout << "AntTweakBar UI scaling factor: " << UI_scalingFactor << std::endl;

	std::string UI_scalingStr = "GLOBAL fontscaling=" + std::to_string(UI_scalingFactor);
	TwDefine(UI_scalingStr.c_str()); // change the size of all UI elements in the AntTweakBar
}


void printMatrixArray(const glm::mat4x4& mat) {
	// Pointer to the underlying data of the matrix
	const float* ptr = glm::value_ptr(mat);

	// Output all 16 values in the order they are stored in memory
	for (int i = 0; i < 16; ++i) {
		std::cout << ptr[i] << " ";
	}
	std::cout << std::endl;
}


void printMatrix(const glm::mat4x4& mat) {
	// Pointer to the underlying data of the matrix
	const float* ptr = glm::value_ptr(mat);

	// Print the matrix row by row
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			std::cout << ptr[col * 4 + row] << " ";
		}
		std::cout << std::endl; // New line after each row
	}
}


//helper function to convert a quaternion to a 4x4 rotation matrix
void ConvertQuaternionToMatrix(const float quat[4], glm::mat4x4& mat)
{
	float yy2 = 2.0f * quat[1] * quat[1];
	float xy2 = 2.0f * quat[0] * quat[1];
	float xz2 = 2.0f * quat[0] * quat[2];
	float yz2 = 2.0f * quat[1] * quat[2];
	float zz2 = 2.0f * quat[2] * quat[2];
	float wz2 = 2.0f * quat[3] * quat[2];
	float wy2 = 2.0f * quat[3] * quat[1];
	float wx2 = 2.0f * quat[3] * quat[0];
	float xx2 = 2.0f * quat[0] * quat[0];

	mat[0][0] = -yy2 - zz2 + 1.0f;
	mat[1][0] = xy2 + wz2;
	mat[2][0] = xz2 - wy2;
	mat[3][0] = 0;

	mat[0][1] = xy2 - wz2;
	mat[1][1] = -xx2 - zz2 + 1.0f;
	mat[2][1] = yz2 + wx2;
	mat[3][1] = 0;

	mat[0][2] = xz2 + wy2;
	mat[1][2] = yz2 - wx2;
	mat[2][2] = -xx2 - yy2 + 1.0f;
	mat[3][2] = 0;

	mat[0][3] = 0;
	mat[1][3] = 0;
	mat[2][3] = 0;
	mat[3][3] = 1;
}


void createTranslationMatrix(float tx, float ty, float tz, glm::mat4x4& mat)
{
	mat = glm::mat4(1.0f);
	mat[0][3] = tx;
	mat[1][3] = ty;
	mat[2][3] = tz;
}


void createPerspectiveProjectionMatrix(float nearPlane, float farPlane, float right, float top, glm::mat4x4& mat)
{
	assert(nearPlane > 0.0f);
	assert(farPlane > nearPlane);
	assert(farPlane > nearPlane);
	assert(right > 0.0f);
	assert(top > 0.0f);

	mat = glm::mat4(0.0f);

	mat[0][0] = nearPlane / right;

	mat[1][1] = nearPlane / top;
	
	mat[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
	mat[2][3] = -2.0f * (farPlane * nearPlane) / (farPlane - nearPlane);
	
	mat[3][2] = -1.0f;

}


void displayMessage(const std::string& str)
{
	std::wstring wStr(str.begin(), str.end());
	MessageBox(NULL, wStr.c_str(), TEXT("Renderer"), MB_OK);
}


std::wstring getOpenFileName()
{
	const int strMaxLen = 10000;
	OPENFILENAME ofn = {0};
	TCHAR fileStr[strMaxLen] = {0};

	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = fileStr;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(fileStr)/sizeof(TCHAR) - 1;

	GetOpenFileName(&ofn);
	return fileStr;
}




std::string readShaderSource(const std::string& shaderFileName)
{
	std::ifstream ifile(shaderFileName);
	std::string filetext;

	while(ifile.good())
	{
		std::string line;
		std::getline(ifile, line);
		filetext.append(line + "\n");
	}
	ifile.close();
	return filetext;
}




void createCube(std::vector<glm::vec4>& positions, std::vector<glm::vec4>& colors, std::vector<glm::uvec3>& triangleIndices)
{
	//8 corners of a unit cube
	positions.clear();
	positions.reserve(8);
	positions.push_back(glm::vec4(-0.5, -0.5, 0.5, 1.0));
	positions.push_back(glm::vec4(-0.5, 0.5, 0.5, 1.0));
	positions.push_back(glm::vec4(0.5, 0.5, 0.5, 1.0));
	positions.push_back(glm::vec4(0.5, -0.5, 0.5, 1.0));
	positions.push_back(glm::vec4(-0.5, -0.5, -0.5, 1.0));
	positions.push_back(glm::vec4(-0.5, 0.5, -0.5, 1.0));
	positions.push_back(glm::vec4(0.5, 0.5, -0.5, 1.0));
	positions.push_back(glm::vec4(0.5, -0.5, -0.5, 1.0));

	//8 RGBA vertex colors - one for each vertex of the cube
	colors.clear();
	colors.reserve(8);
	colors.push_back(glm::vec4(0.0, 0.0, 0.0, 1.0));  // black;
	colors.push_back(glm::vec4(1.0, 0.0, 0.0, 1.0));  // red;
	colors.push_back(glm::vec4(1.0, 1.0, 0.0, 1.0));  // yellow;
	colors.push_back(glm::vec4(0.0, 1.0, 0.0, 1.0));  // green;
	colors.push_back(glm::vec4(0.0, 0.0, 1.0, 1.0));  // blue;
	colors.push_back(glm::vec4(1.0, 0.0, 1.0, 1.0));  // magenta;
	colors.push_back(glm::vec4(1.0, 1.0, 1.0, 1.0));  // white;
	colors.push_back(glm::vec4(0.0, 1.0, 1.0, 1.0));  // cyan;

	//generate 12 triangles: 36 indices
	triangleIndices.clear();
	triangleIndices.reserve(12);
	triangleIndices.push_back(glm::uvec3(1, 0, 3));
	triangleIndices.push_back(glm::uvec3(1, 3, 2));
	triangleIndices.push_back(glm::uvec3(2, 3, 7));
	triangleIndices.push_back(glm::uvec3(2, 7, 6));
	triangleIndices.push_back(glm::uvec3(3, 0, 4));
	triangleIndices.push_back(glm::uvec3(3, 4, 7));
	triangleIndices.push_back(glm::uvec3(6, 5, 1));
	triangleIndices.push_back(glm::uvec3(6, 1, 2));
	triangleIndices.push_back(glm::uvec3(4, 5, 6));
	triangleIndices.push_back(glm::uvec3(4, 6, 7));
	triangleIndices.push_back(glm::uvec3(5, 4, 0));
	triangleIndices.push_back(glm::uvec3(5, 0, 1));
}


//create a GLSL program object from vertex and fragment shader files
GLuint initShader(std::string vertexShaderFileName, std::string fragmentShaderFileName)
{
	Shader shaders[2] =
	{
		{vertexShaderFileName, GL_VERTEX_SHADER, std::string()},
		{fragmentShaderFileName, GL_FRAGMENT_SHADER, std::string()}
	};

	GLuint program = glCreateProgram();

	for ( int i = 0; i < 2; ++i ) {
		Shader& s = shaders[i];
		s.source = readShaderSource( s.filename );
		if (shaders[i].source.empty())
		{
			std::cerr << "Failed to read " << s.filename << std::endl;
			return 0;
		}

		GLuint shader = glCreateShader( s.type );
		const GLchar* strings[1];
		strings[0] = s.source.c_str();
		glShaderSource(shader, 1, strings, NULL);
		
		glCompileShader( shader );

		GLint  compiled;
		glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
		if ( !compiled ) {
			std::cerr << s.filename << " failed to compile:" << std::endl;
			GLint  logSize = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
			char* logMsg = new char[logSize];
			glGetShaderInfoLog(shader, logSize, NULL, logMsg);
			std::cerr << logMsg << std::endl;
			delete [] logMsg;
			return 0;
		}

		glAttachShader(program, shader);
	}

	/* link  and error check */
	glLinkProgram(program);

	GLint  linked;
	glGetProgramiv( program, GL_LINK_STATUS, &linked );
	if ( !linked ) {
		std::cerr << "Shader program failed to link" << std::endl;
		GLint  logSize;
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
		char* logMsg = new char[logSize];
		glGetProgramInfoLog( program, logSize, NULL, logMsg );
		std::cerr << logMsg << std::endl;
		delete [] logMsg;

		return 0;
		//exit( EXIT_FAILURE );
	}

	/* use program object */
	glUseProgram(program);

	return program;
}


void displaySoftwareStatus()
{
	// Query OpenGL version
	const char* version = (const char*)glGetString(GL_VERSION);
	std::cout << "OpenGL Version: " << version << std::endl;

	// Query GLSL version
	const char* glslVersion = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	std::cout << "GLSL Version: " << glslVersion << std::endl;

	// Query profile
	int profileMask;
	glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profileMask);
	if (profileMask & GL_CONTEXT_CORE_PROFILE_BIT) {
		std::cout << "Profile: Core" << std::endl;
	}
	else if (profileMask & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) {
		std::cout << "Profile: Compatibility" << std::endl;
	}
	else {
		std::cout << "Profile: Unknown" << std::endl;
	}

	// Query context flags
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT) {
		std::cout << "Forward Compatible Context: Yes" << std::endl;
	}
	else {
		std::cout << "Forward Compatible Context: No" << std::endl;
	}

	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		std::cout << "Debug Context: Yes" << std::endl;
	}
	else {
		std::cout << "Debug Context: No" << std::endl;
	}

	if (glIsEnabled(GL_DEBUG_OUTPUT)) {
		std::cout << "Debug Output: Yes" << std::endl;
	}
	else {
		std::cout << "Debug Output: No" << std::endl;
	}

	// Query FreeGLUT version
	int majorVersion = glutGet(GLUT_VERSION) / 10000;
	int minorVersion = (glutGet(GLUT_VERSION) / 100) % 100;
	int patchVersion = glutGet(GLUT_VERSION) % 100;

	std::cout << "FreeGLUT Version: "
		<< majorVersion << "."
		<< minorVersion << "."
		<< patchVersion << std::endl;

	std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

	int glmVersion = GLM_VERSION;
	int major = glmVersion / 1000;
	int minor = (glmVersion / 100) % 10;
	int patch = glmVersion % 100;

	std::cout << "GLM version: " << major << "." << minor << "." << patch << std::endl;

	const char* FreeImageVersion = FreeImage_GetVersion();
	std::cout << "FreeImage version: " << FreeImageVersion << std::endl;

	std::cout << "--------------------------------" << std::endl;
}


bool LoadImage(const char* filename, BYTE** bits, unsigned int& width, unsigned int& height, FREE_IMAGE_COLOR_TYPE& colorType)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP* dib(0);

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
	{
		fif = FreeImage_GetFIFFromFilename(filename);
	}
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
	{
		return false;
	}

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
	{
		dib = FreeImage_Load(fif, filename);
		dib = FreeImage_ConvertTo32Bits(dib);

	}
	//if the image failed to load, return failure
	if (!dib)
	{
		return false;
	}

	//retrieve the image data
	*bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((*bits == 0) || (width == 0) || (height == 0))
	{
		return false;
	}

	colorType = FreeImage_GetColorType(dib);

	return true;
}


