#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>


#include <vector>
#include <Windows.h>
#include <assert.h>
#include <algorithm>


#include "Utils/Utils.h"
#include "Renderer.h"
#include "Utils/wavefront_obj.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

LARGE_INTEGER ElapsedMicroseconds;


//points coordinates
int g_P1x = 20, g_P1y = 20; //P1
int g_P2x = 500, g_P2y = 400; //P2

//obj data type
Wavefront_obj objScene;

//in general, using global variables is a bad practice. This is used here for simplicity and for demonstration purposes only. Remove this and use local variables in your implementation.
BYTE* imageRawData = 0;
unsigned int imageWidth = 0;
unsigned int imageHeight = 0;
FREE_IMAGE_COLOR_TYPE colorType = FIC_RGB;
GLuint g_programID = 0;

Renderer renderer(1, 1);

//forward declarations
void TW_CALL loadOBJModel(void* clientData);
void initScene();
void initGraphics(int argc, char *argv[]);
void drawScene();
void Display();
void Reshape(int width, int height);
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void PassiveMouseMotion(int x, int y);
void Keyboard(unsigned char k, int x, int y);
void Special(int k, int x, int y);
void Terminate(void);



int main(int argc, char *argv[])
{
	// Initialize openGL, glut, glew
	initGraphics(argc, argv);

	antTeakBar_UIscale();

	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);


	// Set GLUT callbacks
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(PassiveMouseMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	
	//send 'glutGetModifers' function pointer to AntTweakBar.
	//required because the GLUT key event functions do not report key modifiers states.
	//TwGLUTModifiersFunc(glutGetModifiers);


	atexit(Terminate);  //called after glutMainLoop ends

	// Create a tweak bar
	TwBar* bar = TwNewBar("TweakBar");


	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='350 800' color='96 216 224' "); // change default tweak bar size and color
	TwAddButton(bar, "open", loadOBJModel, NULL, " label='Open OBJ File...' ");

	//read only - displaying the draw timer in micro seconds
	TwAddVarRO(bar, "time (us)", TW_TYPE_UINT32, &ElapsedMicroseconds.LowPart, "help='shows the drawing time in micro seconds'");
	//the x,y coordinates
	TwAddVarRW(bar, "P1.x", TW_TYPE_UINT16, &g_P1x, " min=0 max=10000 step=1 keyIncr=x keyDecr=X help='Point 1 x coordinate' ");
	TwAddVarRW(bar, "P1.y", TW_TYPE_UINT16, &g_P1y, " min=0 max=10000 step=1 keyIncr=y keyDecr=Y help='Point 1 y coordinate' ");
	TwAddVarRW(bar, "P2.x", TW_TYPE_UINT16, &g_P2x, " min=0 max=10000 step=1 keyIncr=a keyDecr=A help='Point 2 x coordinate' ");
	TwAddVarRW(bar, "P2.y", TW_TYPE_UINT16, &g_P2y, " min=0 max=10000 step=1 keyIncr=b keyDecr=B help='Point 2 y coordinate' ");


	
	LoadImage("..\\Images\\Einstein Rendered.jpg", &imageRawData, imageWidth, imageHeight, colorType);
	//LoadImage("..\\Images\\GPU with alpha channel.png", &imageRawData, imageWidth, imageHeight, colorType);
	//LoadImage("..\\Images\\lion 4k.jpg", &imageRawData, imageWidth, imageHeight, colorType);


	/// ////////////////////////////////////////////////////////////////////////
	//this code is just for demonstration of glm use. You can delete it.
	glm::dmat4x4 model(1.0); //init to identity matrix
	glm::vec4 vec(1.0, 1.0, 1.0, 1.0);
	glm::vec4 res = model * vec;
	res = vec * model;
	/// ////////////////////////////////////////////////////////////////////////


	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}


void TW_CALL loadOBJModel(void *data)
{
	std::wstring str = getOpenFileName();

	bool result = objScene.load_file(str);

	if(result)
	{
		std::cout << "The obj file was loaded successfully" << std::endl;
	}
	else
	{
		std::cerr << "Failed to load obj file" << std::endl;
	}

	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;

}


void initGraphics(int argc, char* argv[])
{

	// Set the DPI awareness to Per-Monitor DPI Awareness
	bool res = SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);


	// Initialize GLUT
	glutInit(&argc, argv);

	glutInitContextVersion(4, 6); //latest most advanced version of OpenGL
	glutInitContextProfile(GLUT_CORE_PROFILE); // Request a core profile
	
	//for some reason, the forward compatible mode causes errors on my surface laptop with Nvidia card. It might be related to AntTweakBar
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE); // Request forward-compatible context
	//
	// Request a debug context
	//glutInitContextFlags(GLUT_DEBUG);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); //use a framebuffer with double buffering and RGBA format. Use a z-buffer

	glutInitWindowSize(1600, 900);
	glutCreateWindow("Computer Graphics Skeleton using: AntTweakBar, freeGlut, glew");
	glutCreateMenu(NULL);

	initGlew();

	debugOutputOpenGL();

	displaySoftwareStatus();

// 	//create, load, compile, attach vertex and fragment shader
// 	g_programID = initShader("..\\Shaders\\vertexShaderPassThrough.glsl", "..\\Shaders\\fragmentShaderQuadTexture.glsl");
// 	if (!g_programID)
// 	{
// 		std::cout << "\nFatal Error in shader creation!\n\a\a\a";
// 		return;
// 	}
}




//this is just an example of usage for the Renderer::drawPixels function.
// currently, it is used to render (pixel by pixel) an image which is loaded from an external file.
//delete the content of this function and use your own code for drawing the scene.
//it is also a good idea to move the drawScene() function to another file/class
void drawScene()
{
	static int k = 1;

	int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	renderer.clearBufferToZero();

	int width = std::min((unsigned int)windowWidth, imageWidth);
	int height = std::min((unsigned int)windowHeight, imageHeight);

	int numChannels = 0;

	switch (colorType)
	{
	case FIC_RGB:
		numChannels = 3;
		break;

	case FIC_RGBALPHA:
		numChannels = 4;
		break;

	default: //this is currently not supported but can easily changed to support additional color formats
		assert(0);
		exit(1);
		break;
	}

	if (1) //1 shows image, 0 shows red blue stripes

	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				//Calculate the index for the RGB array. This code assumes that there is no alpha channel in the row data of the loaded image file (e.g. jpg).
				//For file format that contains alpha channel (transparency) such as PNG, this should be altered.
				//Assuming that raw data is stored in a row major (as opposed to column major) order and that the first row in memory is the bottom row of the image.

				int indexInImage = (i * imageWidth + j) * numChannels;
				BYTE r = imageRawData[indexInImage];
				BYTE g = imageRawData[indexInImage + 1];
				BYTE b = imageRawData[indexInImage + 2];
				BYTE a = 255; //use 255 as default alpha channel
				if (colorType == FIC_RGBALPHA)
				{
					a = imageRawData[indexInImage + 3]; //alpha channel
				}

				renderer.addPixel(j, i, r, g, b, a);
			}
		}
	}
	else
	{
		for (int i = 0; i < windowHeight; i++)
		{
			for (int j = 0; j < windowWidth; j++)
			{
				if (j % 2)
				{
					renderer.addPixel(j, i, 255, 0, 0, 255);
				}
				else
				{
					renderer.addPixel(j, i, 0, 0, 255, 255);
				}
			}
		}
	}

	renderer.drawPixels();
}


// callback function called by GLUT to render screen
void Display()
{
//  for debugging. Testing how many times the Display function is invoked by the application and when it happens
// 	static int counter = 0;
// 	std::cout << "C: " << counter << std::endl;
// 	counter++;

	glClearColor(0.0, 0.0, 0.0, 1.0); //set the background color to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //initialize z-buffer and framebuffer


	LARGE_INTEGER EndingTime;
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER Frequency;


	QueryPerformanceCounter(&StartingTime);
 	drawScene(); //this is the most time consuming function as it is in charge of the rendering process
	//time measuring - don't delete
	QueryPerformanceCounter(&EndingTime);


	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	//initialize the timer frequency
	QueryPerformanceFrequency(&Frequency);

	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	// draws the tweak bar
	TwDraw();

	//swap back and front frame buffers
	glutSwapBuffers();
}


// Callback function called by GLUT when window size changes
void Reshape(int width, int height)
{
	//no need to use glViewport as we are copying the pixels directly without rendering anything
	//glViewport(0, 0, width, height);

	renderer.resizeBuffer(width, height);
	renderer.createTexture(width, height);

    // Send the new window size to AntTweakBar
    TwWindowSize(width, height);
    glutPostRedisplay();
}


void MouseButton(int button, int state, int x, int y)
{
	TwEventMouseButtonGLUT(button, state, x, y);
	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
	glutPostRedisplay();
}

void PassiveMouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
}


void Keyboard(unsigned char k, int x, int y)
{
	TwEventKeyboardGLUT(k, x, y); //do not delete this line!
	glutPostRedisplay();
}

void Special(int k, int x, int y)
{
	TwEventSpecialGLUT(k, x, y);
	glutPostRedisplay();
}


// Function called at exit
void Terminate(void)
{
	//consider freeing up memory here.

    TwTerminate();
}



