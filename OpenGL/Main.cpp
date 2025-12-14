#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>

#include <vector>
#include <Windows.h>
#include <assert.h>

#include "Object.h"
#include "Utils/Utils.h"
#include "Utils/wavefront_obj.h"
#include "TweakBar.h"
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Globals.h"
#include "Bezier.h"
LARGE_INTEGER ElapsedMicroseconds;

 

GLuint g_programID = 0;
GLuint g_gouraudProgramID;
GLuint g_phongProgramID;
GLuint g_textProgramID;
GLuint g_wireFrameProgramID;
GLuint g_deformationProgramID;
GLuint g_curveProgramID;
GLuint curveVAOID = 0;
GLuint curveVBOID = 1;

GLuint controlVAOID = 0;
GLuint controlVBOID = 1;

GLuint vertexArrayObjectID = 0;
GLuint textureID = 0;
GLuint normalID = 0;

bool loaded_texture = false;
bool loaded_normalMap = false;


//obj data type
Wavefront_obj objScene;

void TW_CALL loadOBJModel(void* clientData);
void TW_CALL loadTextureModel(void* clientData);
void TW_CALL loadNormalMapModel(void* clientData);
void initScene();
void initShaders();
void initBezCurve();
int main(int argc, char* argv[]);
void idle();
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


	initShaders();

	atexit(Terminate);  // called after glutMainLoop ends

	// Create a tweak bar
	TwBar* bar = TwNewBar("TweakBar");

	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='350 800' color='96 216 224' "); // change default tweak bar size and color
	TwAddButton(bar, "openobj", loadOBJModel, NULL, " label='Open OBJ File...' ");
	TwAddButton(bar, "opentex", loadTextureModel, NULL, " label='Open Texture File...' ");
	TwAddButton(bar, "openNormal", loadNormalMapModel, NULL, " label='Open Normal map File...' ");

	//read only - displaying the draw timer in micro seconds
	TwAddVarRO(bar, "time (us)", TW_TYPE_UINT32, &ElapsedMicroseconds.LowPart, "help='shows the drawing time in micro seconds'");
	
	TwEnumVal lightEV[NUM_LIGHT_TYPES] = { { DIRECTIONAL ,"Directional"}, {POINT_LIGHT,"Point light"} };
	TwType lightType = TwDefineEnum("LightType", lightEV, NUM_LIGHT_TYPES);

	TwEnumVal ShadingEV[NUM_SHADING_TYPES] = { { NONE ,"None"},{GOURAUD,"Gouraud"},{PHONG,"Phong"},
		{TEXTURE,"Texture"},{DEFORMATION,"Deformation"} };
	TwType sType = TwDefineEnum("ShadingType", ShadingEV, NUM_SHADING_TYPES);

	TwEnumVal camEV[NUM_CAM_TYPES] = { { FOV ,"FOV"},{FRUSTRUM,"FRUSTRUM"}};
	TwType cType = TwDefineEnum("camType", camEV, NUM_CAM_TYPES);

	TwEnumVal interEV[NUM_INTER_TYPES] = { { SLERP ,"SLERP"},{EULER,"EULER"} };
	TwType iType = TwDefineEnum("interpolation type", interEV, NUM_INTER_TYPES);

	TwEnumVal defEV[NUM_DEF_TYPES] = { { FULL ,"FULL"},{HALF,"HALF"} };
	TwType dType = TwDefineEnum("deformation type", defEV, NUM_DEF_TYPES);

	TwEnumVal disEV[NUM_DIS_TYPES] = { { STATIC ,"Static"},{ANIMATION,"Animation"} };
	TwType disType = TwDefineEnum("display type", disEV, NUM_DIS_TYPES);


	TwEnumVal rotEV[NUM_ROT_TYPES] = { { QUATERNIONS ,"Quaternios"},{ANGLES,"Angles"} };
	TwType rotType = TwDefineEnum("rotation type", rotEV, NUM_ROT_TYPES);

	TwAddVarRW(bar, "Display type", disType, &g_disType, "");

	TwAddVarRW(bar, "play animation", TW_TYPE_BOOL8, &g_play, "Group = 'Animation' key = 'space'");
	TwAddVarRW(bar, "animation speed", TW_TYPE_FLOAT, &g_speed, "Group='Animation' min=0 max=20 step=0.1 help='time for an animation loop' ");
	TwAddVarRW(bar, "interpolation progress", TW_TYPE_FLOAT, &g_time, "Group='Animation' min=0 max=1 step=0.01 help='point of animation' ");
	TwAddVarRW(bar, "interpolation type", iType, &g_IType, "Group = 'Animation'");
	TwAddVarRW(bar, "Rotation Entry type", rotType, &g_rotType, "Group='Animation'");
	TwAddVarRW(bar, "SRAX", TW_TYPE_FLOAT, &g_SR[0], "Group = 'Animation' min=-180 max=180 step=0.5 Label = 'Starting Rotation X'");
	TwAddVarRW(bar, "SRAY", TW_TYPE_FLOAT, &g_SR[1], "Group = 'Animation' min=-180 max=180 step=0.5 Label = 'Starting Rotation Y'");
	TwAddVarRW(bar, "SRAZ", TW_TYPE_FLOAT, &g_SR[2], "Group = 'Animation' min=-180 max=180 step=0.5 Label = 'Starting Rotation Z'");
	TwAddVarRW(bar, "ERAX", TW_TYPE_FLOAT, &g_ER[0], "Group = 'Animation' min=-180 max=180 step=0.5 Label = 'End Rotation X'");
	TwAddVarRW(bar, "ERAY", TW_TYPE_FLOAT, &g_ER[1], "Group = 'Animation' min=-180 max=180 step=0.5 Label = 'End Rotation Y'");
	TwAddVarRW(bar, "ERAZ", TW_TYPE_FLOAT, &g_ER[2], "Group = 'Animation' min=-180 max=180 step=0.5 Label = 'End Rotation Z'");
	TwAddVarRW(bar, "SRQ", TW_TYPE_QUAT4F, &g_startQ, "Group = 'Animation'  Label = 'Starting Rotation'");
	TwAddVarRW(bar, "SEQ", TW_TYPE_QUAT4F, &g_endQ, "Group = 'Animation' Label = 'End Rotation'");




	TwAddVarRW(bar, "snap to curve", TW_TYPE_BOOL8, &g_snap, "Group='Bezier Curve' ");
	TwAddVarRW(bar, "Display curve", TW_TYPE_BOOL8, &g_displayCurve, "Group='Bezier Curve' ");
	TwAddVarRW(bar, "number of samples", TW_TYPE_UINT16, &g_numSegs, "Group='Bezier Curve' min=2 max=100 step=1");
	TwAddVarRW(bar, "Point 1 x", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(0).x, "Group='Bezier Curve' min=-1000 max=1000 step=0.5 ");
	TwAddVarRW(bar, "Point 1 y", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(0).y, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 1 z", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(0).z, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 2 x", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(1).x, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 2 y", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(1).y, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 2 z", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(1).z, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 3 x", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(2).x, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 3 y", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(2).y, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 3 z", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(2).z, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 4 x", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(3).x, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 4 y", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(3).y, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");
	TwAddVarRW(bar, "Point 4 z", TW_TYPE_FLOAT, &g_bezCurve.controlPoints.at(3).z, "Group='Bezier Curve' min=-1000 max=1000 step=0.5");

	TwAddVarRW(bar, "Shader", sType, &g_SType, "Group= 'Shading'");
	TwAddVarRW(bar, "Deformation type", dType, &g_dType, "Group = Shading");
	TwAddVarRW(bar, "Line Width", TW_TYPE_FLOAT, &g_lineW, "Group = Shading min=0.1 max=10 step=0.1");
	TwAddVarRW(bar, "Max deformation", TW_TYPE_FLOAT, &g_maxD, "Group='Shading' min=0.1 max=10 step=0.1 help='max object deformation' ");
	TwAddVarRW(bar, "Deformation time", TW_TYPE_FLOAT, &g_DT, "Group='Shading' min=0.1 max=20 step=0.1");
	TwAddVarRW(bar, "Add Texture", TW_TYPE_BOOL8, &g_texturise, "Group='Shading'");
	TwAddVarRW(bar, "Add Normal Map", TW_TYPE_BOOL8, &g_map, "Group='Shading'");

	TwAddVarRW(bar, "Object Color", TW_TYPE_COLOR32, &g_OColor, "Group='Object Properties' help='object color' ");
	TwAddVarRW(bar, "K Ambient", TW_TYPE_FLOAT, &g_OKa, "min=0 max=1 step=0.1 Group='Object Properties' help='object K ambient' ");
	TwAddVarRW(bar, "K Diffuse", TW_TYPE_FLOAT, &g_OKd, "min=0 max=1 step=0.1 Group='Object Properties' help='object K Diffuse' ");
	TwAddVarRW(bar, "K Specular", TW_TYPE_FLOAT, &g_OKs, "min=0 max=1 step=0.1 Group='Object Properties' help='object K Specular' ");
	TwAddVarRW(bar, "Shininess Coefficient ", TW_TYPE_INT16, &g_OSc, "min=2 max=200 step=2 Group='Object Properties' help='object K Specular' ");
	TwAddVarRW(bar, "Double sided", TW_TYPE_BOOL8, &g_DSCB, "Group='Object Properties' ");

	TwAddVarRW(bar, "Ambient Light Intensity", TW_TYPE_COLOR3F, &g_LAMI, "Group='Lights' help='Change Ambient intensity.' ");
	TwAddVarRW(bar, "Light 1 type", lightType, &g_L1T, "Group='Lights' help='Change light1 type.' ");
	TwAddVarRW(bar, "Light 1 Intensity", TW_TYPE_COLOR3F, &g_L1I, "Group='Lights' help='Change light1 intensity.' ");
	TwAddVarRW(bar, "Light 1 Direction", TW_TYPE_DIR3D, &g_L1D, "Group='Lights' help='Change light1 type.' ");
	TwAddVarRW(bar, "Light 1 X", TW_TYPE_DOUBLE, &g_L1P[0], "min=-1000 max=1000 step=1 Group=Lights Label='light 1 x'");
	TwAddVarRW(bar, "Light 1 Y", TW_TYPE_DOUBLE, &g_L1P[1], "min=-1000 max=1000 step=1 Group=Lights Label='light 1 y'");
	TwAddVarRW(bar, "Light 1 Z", TW_TYPE_DOUBLE, &g_L1P[2], "min=-1000 max=1000 step=1 Group=Lights Label='light 1 z'");

	TwAddVarRW(bar, "DL2", TW_TYPE_BOOL8, &g_L2CB, "Group='Lights' Label='Add second light'");
	TwAddVarRW(bar, "Light 2 Intensity", TW_TYPE_COLOR3F, &g_L2I, "Group='Lights' help='Change light2 intensity.' ");
	TwAddVarRW(bar, "Light 2 type", lightType, &g_L2T, "Group='Lights' help='Change light2 type.' ");
	TwAddVarRW(bar, "Light 2 Direction", TW_TYPE_DIR3D, &g_L2D, "Group='Lights' help='Change light1 type.' ");
	TwAddVarRW(bar, "Light 2 X", TW_TYPE_DOUBLE, &g_L2P[0], "min=-1000 max=1000 step=1 Group=Lights Label='light 2 x'");
	TwAddVarRW(bar, "Light 2 Y", TW_TYPE_DOUBLE, &g_L2P[1], "min=-1000 max=1000 step=1 Group=Lights Label='light 2 y'");
	TwAddVarRW(bar, "Light 2 Z", TW_TYPE_DOUBLE, &g_L2P[2], "min=-1000 max=1000 step=1 Group=Lights Label='light 2 z'");

	
	TwAddVarRW(bar, "F.m", cType, &g_cType, "Group='Frustrom' Label='Camera Mode'");
	TwAddVarRW(bar, "F.n", TW_TYPE_FLOAT, &g_Fn, " min=0 max=1000 step=0.1 Group='Frustrom' Label=near");
	TwAddVarRW(bar, "F.f", TW_TYPE_FLOAT, &g_Ff, " min=0 max=2000 step=1 Group='Frustrom' Label=far");
	TwAddVarRW(bar, "F.r", TW_TYPE_FLOAT, &g_Fr, " min=0.1 max=750 step=0.1 Group='Frustrom' Label=right");
	TwAddVarRW(bar, "F.t", TW_TYPE_FLOAT, &g_Ft, " min=0.1 max=750 step=0.1 Group='Frustrom' Label=top");
	TwAddVarRW(bar, "F.fov", TW_TYPE_FLOAT, &g_Fov, "min=0.5 max=89.5 step=0.5 Group='Frustrom' Label=FoV");

	TwAddVarRW(bar, "Cx", TW_TYPE_FLOAT, &g_Cx, " min=-1000 max=1000 step=1 Group='Cam Position' Label=x");
	TwAddVarRW(bar, "Cy", TW_TYPE_FLOAT, &g_Cy, " min=-1000 max=1000 step=1 Group='Cam Position' Label=y");
	TwAddVarRW(bar, "Cz", TW_TYPE_FLOAT, &g_Cz, " min=-1000 max=1000 step=1 Group='Cam Position' Label=z");
	TwAddButton(bar, "LkA", lookAtObj, NULL, " label='Look at Object' Group='Cam Position'");

	TwAddVarRW(bar, "Tx", TW_TYPE_FLOAT, &g_Tx, " min=-1000 max=1000 step=1 Group='Translate Object' Label=x");
	TwAddVarRW(bar, "Ty", TW_TYPE_FLOAT, &g_Ty, " min=-1000 max=1000 step=1 Group='Translate Object' Label=y");
	TwAddVarRW(bar, "Tz", TW_TYPE_FLOAT, &g_Tz, " min=-1000 max=1000 step=1 Group='Translate Object' Label=z");
	TwAddButton(bar, "TrO", transObj, NULL, " label='Translate object space' Group='Translate Object'");
	TwAddButton(bar, "TrW", transWorld, NULL, " label='Translate world space' Group='Translate Object'");

	TwAddVarRW(bar, "Os", TW_TYPE_FLOAT, &g_Os, " min=0 max=100 step=0.1 Group='Scale Object' Label=scale");
	TwAddButton(bar, "OsO", ScaleObj, NULL, " label='Scale object space' Group='Scale Object'");
	TwAddButton(bar, "OsW", ScaleWorld, NULL, " label='Scale world space' Group='Scale Object'");

	TwAddVarRW(bar, "Rz", TW_TYPE_FLOAT, &g_Rz, " min=-180 max=180 step=1 Group='Rotate Object' Label=Rz");
	TwAddButton(bar, "RzO", RotZObj, NULL, " label='Rotate Z object space' Group='Rotate Object'");
	TwAddButton(bar, "RzW", RotZWorld, NULL, " label='Rotate Z world space' Group='Rotate Object'");

	TwAddVarRW(bar, "Ry", TW_TYPE_FLOAT, &g_Ry, " min=-180 max=180 step=1 Group='Rotate Object' Label=Ry");
	TwAddButton(bar, "RyO", RotYObj, NULL, " label='Rotate Y object space' Group='Rotate Object'");
	TwAddButton(bar, "RyW", RotYWorld, NULL, " label='Rotate Y world space' Group='Rotate Object'");

	TwAddVarRW(bar, "Rx", TW_TYPE_FLOAT, &g_Rx, " min=-180 max=180 step=1 Group='Rotate Object' Label=Rx");
	TwAddButton(bar, "RxO", RotXObj, NULL, " label='Rotate X object space' Group='Rotate Object'");
	TwAddButton(bar, "RxW", RotXWorld, NULL, " label='Rotate X world space' Group='Rotate Object'");

	startUpOptions();//load relevant bar options

	// Call the GLUT main loop
	glutIdleFunc(idle);
	glutMainLoop();

	return 0;
}


void idle() {
	glutPostRedisplay(); // Trigger a redraw
}

void TW_CALL loadOBJModel(void *data)
{

	std::wstring str = getOpenFileName();
	objScene = Wavefront_obj();
	bool result = objScene.load_file(str);

	if(result)
	{
		obj = Object(objScene);
		obj.cam.setViewMat(g_width, g_height);
		obj.sWidth = g_width;
		obj.sHeight = g_height;
		obj.calcModelViewMat();
		objectLoadedOptions();
		std::cout << "The obj file was loaded successfully" << std::endl;
		initScene();
		initBezCurve();
		g_programID = g_phongProgramID;
	}
	else
	{
		std::cerr << "Failed to load obj file" << std::endl;
	}

	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;
}

void TW_CALL loadTextureModel(void* clientData)
{
	std::wstring str = getOpenFileName();
	std::string fileName;
	fileName.resize(str.size());
	for (int i = 0; i < str.size();i++) {
		auto& character = str.at(i);
		fileName.at(i) = (character);
	}
	BYTE* imageRawData = 0;
	unsigned int imageWidth = 0;
	unsigned int imageHeight = 0;
	FREE_IMAGE_COLOR_TYPE colorType;

	LoadImage(fileName.c_str(), &imageRawData, imageWidth, imageHeight, colorType);

	if (imageWidth != 0)
	{
		loaded_texture = true;
	}
	else
	{
		return;
	}

	glActiveTexture(GL_TEXTURE2);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);


	GLint format = GL_BGRA;
	if (colorType == FIC_RGBALPHA) {
		format = GL_RGBA;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0,
		format, GL_UNSIGNED_BYTE, imageRawData);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, textureID);
}


void TW_CALL loadNormalMapModel(void* clientData)
{
	std::wstring str = getOpenFileName();
	std::string fileName;
	fileName.resize(str.size());
	for (int i = 0; i < str.size(); i++) {
		auto& character = str.at(i);
		fileName.at(i) = (character);
	}
	BYTE* imageRawData = 0;
	unsigned int imageWidth = 0;
	unsigned int imageHeight = 0;
	FREE_IMAGE_COLOR_TYPE colorType;

	LoadImage(fileName.c_str(), &imageRawData, imageWidth, imageHeight, colorType);

	if (imageWidth != 0)
	{
		loaded_normalMap = true;
	}
	else
	{
		return;
	}

	
	glActiveTexture(GL_TEXTURE3);
	glGenTextures(1, &normalID);
	glBindTexture(GL_TEXTURE_2D, normalID);


	
	GLint format = GL_BGRA;
	if (colorType == FIC_RGBALPHA) {
		format = GL_RGBA;
	}

	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	
	glTexImage2D(GL_TEXTURE_2D, 0, format, imageWidth, imageHeight, 0,
		format, GL_UNSIGNED_BYTE, imageRawData);



	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, normalID);
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
	glutCreateWindow("Computer Graphics Skeleton using: OpenGL, AntTweakBar, freeGlut, glew");
	glutCreateMenu(NULL);

	// Initialize OpenGL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	initGlew();

	debugOutputOpenGL();

	displaySoftwareStatus();
}



void drawScene()
{
	if (g_programID == 0)
	{
		return;
	}
	TwDefine("TweakBar/'Line Width' visible=false");
	TwDefine("TweakBar/'Deformation type' visible=false");
	TwDefine("TweakBar/'Deformation time' visible=false");
	TwDefine("TweakBar/'Max deformation' visible=false");
	TwDefine("TweakBar/'Add Texture' visible=false");
	TwDefine("TweakBar/'Add Normal Map' visible=false");



	switch (g_SType)
	{
	case GOURAUD:
		g_programID = g_gouraudProgramID;

		break;
	case PHONG:
		g_programID = g_phongProgramID;
		break;
	case NONE:
		g_programID = g_wireFrameProgramID;
		glDisable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(g_lineW); // Adjust line thickness here
		TwDefine("TweakBar/'Line Width' visible=true");
		break;
	case TEXTURE:
		g_programID = g_textProgramID;
		TwDefine("TweakBar/'Add Texture' visible=true");
		TwDefine("TweakBar/'Add Normal Map' visible=true");
		break;
	case DEFORMATION:
		g_programID = g_deformationProgramID;
		TwDefine("TweakBar/'Deformation type' visible=true");
		TwDefine("TweakBar/'Max deformation' visible=true");
		TwDefine("TweakBar/'Deformation time' visible=true");
	default:
		break;
	}
	
	if (g_disType == STATIC)
	{
		g_displayCurve = false;
		TwDefine("TweakBar/'Animation' visible=false");
		TwDefine("TweakBar/'Bezier Curve' visible=false");
		g_snap = false;
	}
	else 
	{
		TwDefine("TweakBar/'Animation' visible=true");
	}
	glm::mat4x4 mat_modelview;
	glm::mat4x4 mat_rotation = glm::mat4(1.0f);
	glm::mat4x4 mat_translation = glm::mat4(1.0f);

	if (g_snap)
	{
		mat_modelview = glm::mat4(1.0f);
	}
	else
	{
		mat_modelview = obj.worldMat;
	}
	float top;
	float right;
	glUseProgram(g_programID);
	

	if (g_programID == g_textProgramID)
	{
		
		GLuint texMap_ID = glGetUniformLocation(g_programID, "texMap");
		GLuint normalMap_ID = glGetUniformLocation(g_programID, "normalMap");
		glUniform1i(texMap_ID,2 );  // Unit 0 for diffuse
		glUniform1i(normalMap_ID, 3);   // Unit 1 for normal map

		GLuint texturise_id = glGetUniformLocation(g_programID, "texturise");
		glUniform1f(texturise_id, g_texturise);

		GLuint map_id = glGetUniformLocation(g_programID, "map");
		glUniform1f(map_id, g_map);

		GLuint has_texture_id = glGetUniformLocation(g_programID, "has_texture");
		glUniform1f(has_texture_id, loaded_texture);

		GLuint has_normal_id = glGetUniformLocation(g_programID, "has_normal");
		glUniform1f(has_normal_id, loaded_normalMap);
	}

	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float aspectRatio = (float)width / (float)height;
	obj.cam.aspect_ratio = aspectRatio;
	if (width > height)
	{
		top = g_Ft;
		right = aspectRatio * top;
	}
	else
	{
		right = g_Fr;
		top = right / aspectRatio;
	}

	if (g_CamM)
	{
		obj.cam.setPrespMat(g_Fn, g_Ff, g_Fov);
	}
	else {
		obj.cam.setPrespMat(g_Fn, g_Ff, right, top);
	}
	obj.cam.moveCam(g_Cx, g_Cy, g_Cz);
	obj.calcPos();
	obj.lights.at(0) = Light(AMBIENT, g_LAMI);
	if (g_L1T == DIRECTIONAL)
	{
		TwDefine("TweakBar/'Light 1 Direction' visible=true");
		TwDefine("TweakBar/'Light 1 X' visible=false");
		TwDefine("TweakBar/'Light 1 Y' visible=false");
		TwDefine("TweakBar/'Light 1 Z' visible=false");
		obj.lights.at(1) = Light(g_L1T, g_L1D, g_L1I);
	}
	else
	{
		TwDefine("TweakBar/'Light 1 Direction' visible=false");
		TwDefine("TweakBar/'Light 1 X' visible=true");
		TwDefine("TweakBar/'Light 1 Y' visible=true");
		TwDefine("TweakBar/'Light 1 Z' visible=true");
		obj.lights.at(1) = Light(g_L1T, g_L1P, g_L1I);
	}
	if (g_L2CB)
	{
		TwDefine("TweakBar/'Light 2 Intensity' visible=true");
		TwDefine("TweakBar/'Light 2 type' visible=true");

		if (g_L2T == DIRECTIONAL)
		{
			TwDefine("TweakBar/'Light 2 Direction' visible=true");
			TwDefine("TweakBar/'Light 2 X' visible=false");
			TwDefine("TweakBar/'Light 2 Y' visible=false");
			TwDefine("TweakBar/'Light 2 Z' visible=false");
			obj.lights.at(2) = Light(g_L2T, g_L2D, g_L2I);
		}
		else
		{
			TwDefine("TweakBar/'Light 2 Direction' visible=false");
			TwDefine("TweakBar/'Light 2 X' visible=true");
			TwDefine("TweakBar/'Light 2 Y' visible=true");
			TwDefine("TweakBar/'Light 2 Z' visible=true");
			obj.lights.at(2) = Light(g_L2T, g_L2P, g_L2I);
		}
	}
	else
	{
		TwDefine("TweakBar/'Light 2 type' visible=False");
		TwDefine("TweakBar/'Light 2 Intensity' visible=false");
		TwDefine("TweakBar/'Light 2 Direction' visible=false");
		TwDefine("TweakBar/'Light 2 X' visible=false");
		TwDefine("TweakBar/'Light 2 Y' visible=false");
		TwDefine("TweakBar/'Light 2 Z' visible=false");
		obj.lights.at(2) = Light(OFF, g_L2P, g_L2I);
	}
	float t;
	float speed;
	float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	if (g_play)
	{
		
		speed = 10*1/g_speed;
		t = glm::mod(time, speed) / (speed * 0.5);
		t = glm::abs(t - 1.0);
		g_time = t;


	}
	else
	{	
		t = g_time;
		speed = 0;
	}
	
	GLuint mat_modelView_id = glGetUniformLocation(g_programID, "modelView");
	glUniformMatrix4fv(mat_modelView_id, 1, false, glm::value_ptr(mat_modelview));

	if (g_SType == DEFORMATION)
	{
		GLuint maxD_id = glGetUniformLocation(g_programID, "maxD");
		glUniform1f(maxD_id, g_maxD);
		bool full_deform = TRUE;
		if (g_dType == HALF)
		{
			full_deform = FALSE;
		}
		GLuint full_deform_id = glGetUniformLocation(g_programID, "full_deform");
		glUniform1f(full_deform_id, full_deform);

		GLuint time_id = glGetUniformLocation(g_programID, "time");
		glUniform1f(time_id, time);

		GLuint animation_time_id = glGetUniformLocation(g_programID, "animation_time");
		glUniform1f(animation_time_id, g_DT);
	}

	if (g_disType == ANIMATION)
	{
		TwDefine("TweakBar/'Bezier Curve' visible=true");
		glm::quat startQ;
		glm::quat endQ;
		if (g_rotType == QUATERNIONS)
		{
			TwDefine("TweakBar/SRQ visible=true");
			TwDefine("TweakBar/SEQ visible=true");
			TwDefine("TweakBar/SRAX visible=false");
			TwDefine("TweakBar/SRAY visible=false");
			TwDefine("TweakBar/SRAZ visible=false");
			TwDefine("TweakBar/ERAX visible=false");
			TwDefine("TweakBar/ERAY visible=false");
			TwDefine("TweakBar/ERAZ visible=false");

			startQ = { g_startQ[3],g_startQ[0],g_startQ[1],g_startQ[2] };
			endQ = { g_endQ[3],g_endQ[0],g_endQ[1],g_endQ[2] };
		}
		else
		{
			TwDefine("TweakBar/SRQ visible=false");
			TwDefine("TweakBar/SEQ visible=false");
			TwDefine("TweakBar/SRAX visible=true");
			TwDefine("TweakBar/SRAY visible=true");
			TwDefine("TweakBar/SRAZ visible=true");
			TwDefine("TweakBar/ERAX visible=true");
			TwDefine("TweakBar/ERAY visible=true");
			TwDefine("TweakBar/ERAZ visible=true");
			startQ = { glm::quat(glm::radians(g_SR))};
			endQ = { glm::quat(glm::radians(g_ER))};
		}

		glm::vec3 point = g_bezCurve.getPoint(t);
		mat_translation = getTranslation({ point.x,point.y,point.z });

		if (g_IType == SLERP)
		{
			glm::quat pointQuat = glm::slerp(startQ, endQ, t);
			float pointQuatP[4] = { pointQuat[0],pointQuat[1],pointQuat[2],pointQuat[3] };
			ConvertQuaternionToMatrix(pointQuatP, mat_rotation);
		}
		else if (g_IType == EULER)
		{
			glm::vec3 startEuler = glm::eulerAngles(startQ);
			glm::vec3 endEuler = glm::eulerAngles(endQ);
			glm::vec3 degs = glm::mix(startEuler, endEuler, t);
			glm::quat pointQuat = glm::quat(degs);
			float pointQuatP[4] = { pointQuat[0],pointQuat[1],pointQuat[2],pointQuat[3] };
			ConvertQuaternionToMatrix(pointQuatP, mat_rotation);

		}

		
	}
	
	GLuint mat_rotation_id = glGetUniformLocation(g_programID, "rotation");
	glUniformMatrix4fv(mat_rotation_id, 1, false, glm::value_ptr(mat_rotation));

	GLuint mat_translation_id = glGetUniformLocation(g_programID, "translation");
	glUniformMatrix4fv(mat_translation_id, 1, false, glm::value_ptr((mat_translation)));

	GLuint mat_projection_id = glGetUniformLocation(g_programID, "projection");
	glUniformMatrix4fv(mat_projection_id, 1, false, glm::value_ptr((obj.cam.getPrespMat())));
	obj.position = glm::vec4(obj.position,1) * mat_rotation * mat_translation;
	//GLuint scale_id = glGetUniformLocation(g_programID, "scale");
	//glUniform1f(scale_id, g_scale);
	float a = ((g_OColor >> 24) & 0xFF)/255.0f; // Extract Alpha
	float b = ((g_OColor >> 16) & 0xFF) / 255.0f; // Extract Red
	float g = ((g_OColor >> 8) & 0xFF) / 255.0f; // Extract Green
	float r = (g_OColor & 0xFF) / 255.0f;

	glm::vec4 objColor = { a,b,g,r };
	GLuint objColor_id = glGetUniformLocation(g_programID, "objColor");
	glUniform4fv(objColor_id, 1, glm::value_ptr(objColor));

	glm::vec4 objCoeffs = { g_OKa,g_OKd,g_OKs,g_OSc };
	GLuint objCoeffs_id = glGetUniformLocation(g_programID, "objCoeffs");
	glUniform4fv(objCoeffs_id, 1, glm::value_ptr(objCoeffs));

	GLuint doublesided_id = glGetUniformLocation(g_programID, "double_sided");
	glUniform1i(doublesided_id, g_DSCB);

	glm::vec3 camPos = { obj.cam.position};
	GLuint camPos_id = glGetUniformLocation(g_programID, "camPos");
	glUniform3fv(camPos_id, 1, glm::value_ptr(camPos));

	for (int i = 0; i < 3; i++) {
		std::string base = "lights[" + std::to_string(i) + "]";
		GLuint pos_dir = glGetUniformLocation(g_programID, (base + ".pos_dir").c_str());
		GLuint intensity = glGetUniformLocation(g_programID, (base + ".intensity").c_str());
		GLuint type = glGetUniformLocation(g_programID, (base + ".type").c_str());
		glUniform3fv(pos_dir, 1, glm::value_ptr(glm::vec3({ obj.lights[i].pos_dir[0],obj.lights[i].pos_dir[1],obj.lights[i].pos_dir[2] })));
		glUniform3fv(intensity, 1, glm::value_ptr(glm::vec3({ obj.lights[i].intensity[0],obj.lights[i].intensity[1],obj.lights[i].intensity[2] })));
		glUniform1i(type, obj.lights[i].type);
	}

	
	
	int numIndices = obj.geometry.objFaces.size()*3;
	glBindVertexArray(vertexArrayObjectID);

	if (textureID!=0)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	if (normalID !=0)
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalID);
	}

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	glLineWidth(1.0f);
	// Unbind the shader program after drawing
	glBindVertexArray(0);
	glUseProgram(0);

	
	if (g_displayCurve)
	{
		glUseProgram(g_curveProgramID);

		GLuint projection_id = glGetUniformLocation(g_curveProgramID, "projection");
		glUniformMatrix4fv(projection_id, 1, false, glm::value_ptr((obj.cam.getPrespMat())));

		GLuint curveColor_id = glGetUniformLocation(g_curveProgramID, "vColor");
		glUniform4fv(curveColor_id, 1, glm::value_ptr(glm::vec4(1.0f,0.0f,0.0f,0.0f)));
	
		glBindVertexArray(curveVAOID);
		g_bezCurve.sampleCurve(g_numSegs);
		glBindBuffer(GL_ARRAY_BUFFER, curveVBOID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, g_bezCurve.curvePoints.size() * sizeof(glm::vec3), g_bezCurve.curvePoints.data());
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_LINE_STRIP, 0, g_bezCurve.curvePoints.size());
		glBindVertexArray(0);

		GLuint controlColor_id = glGetUniformLocation(g_curveProgramID, "vColor");
		glUniform4fv(curveColor_id, 1, glm::value_ptr(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)));


		glBindVertexArray(controlVAOID);
		glBindBuffer(GL_ARRAY_BUFFER, controlVBOID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, g_bezCurve.controlPoints.size() * sizeof(glm::vec3), g_bezCurve.controlPoints.data());
		glPointSize(15.0f);
		glDrawArrays(GL_POINTS, 0, g_bezCurve.controlPoints.size());
		glBindVertexArray(0);
	
		glUseProgram(0);

	}

	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}


//callback function called by GLUT to render screen
void Display()
{
//  	static int counter = 0;
//  	std::cout << "C: " << counter << std::endl;
//  	counter++;

	glClearColor(0.0, 0.0, 0.0, 1.0); //set the background color to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //init z-buffer and framebuffer


	LARGE_INTEGER EndingTime;
	LARGE_INTEGER StartingTime;
	LARGE_INTEGER Frequency;

	QueryPerformanceCounter(&StartingTime);

	drawScene();

	//time measuring - don't delete
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	//initialize the timer frequency
	QueryPerformanceFrequency(&Frequency);

	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	//draw tweak bars
	TwDraw();

	//swap back and front frame buffers
	glutSwapBuffers();
}


// Callback function called by GLUT when window size changes
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

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
	glutPostRedisplay();
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
	TwTerminate();
	//to do: add code for releasing OpenGL data such as textures and buffers
}


void initScene()
{
	
	//create a Vertex Array Object (VAO)
	glGenVertexArrays(1, &vertexArrayObjectID);
	glBindVertexArray(vertexArrayObjectID);

	GLuint vertexBufferObjectID = 0;
	//create and initialize a Vertex Buffer Object (VBO)
	glGenBuffers(1, &vertexBufferObjectID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjectID);

	int numV = obj.geometry.objPoints.size();
	

	//this will allocate memory for the buffer object on the GPU
	glBufferData(GL_ARRAY_BUFFER, numV*(sizeof(glm::vec4)*2)+ numV * sizeof(glm::vec2) + numV * (sizeof(glm::vec3) * 2), NULL, GL_STATIC_DRAW);
	//this will copy the data from CPU memory to GPU memory
	//glBufferSubData redefines the data store for the buffer object currently bound to target
	glBufferSubData(GL_ARRAY_BUFFER, 0, numV*sizeof(glm::vec4), obj.geometry.objPoints.data());
	//the normals are appended to the buffer right after the positions
	glBufferSubData(GL_ARRAY_BUFFER, numV*sizeof(glm::vec4), numV*sizeof(glm::vec4), obj.geometry.objNormals.data());
	//the text coords are appended to the buffer right after the normals
	glBufferSubData(GL_ARRAY_BUFFER, numV * 2* sizeof(glm::vec4),numV* sizeof(glm::vec2), obj.geometry.objTextCords.data());
	//the tangent coords are appended to the buffer right after the text coords
	glBufferSubData(GL_ARRAY_BUFFER, numV * 2 * sizeof(glm::vec4) + numV * sizeof(glm::vec2), numV * sizeof(glm::vec3), obj.geometry.objTangents.data());
	//the bitangent coords are appended to the buffer right after the tangent
	glBufferSubData(GL_ARRAY_BUFFER, numV * 2 * sizeof(glm::vec4) + numV * sizeof(glm::vec2)+ numV *sizeof(glm::vec3), numV * sizeof(glm::vec3), obj.geometry.objBiTangents.data());





	//get the identifier of the attribute "vPosition" in the active gl program
	GLint vPosition_id = 0;
	//this enables the generic vertex attribute array such that the values in the generic vertex attribute array
	//will be accessed and used for rendering when calls are made to vertex array commands such as glDrawArrays or glDrawElements.
	glEnableVertexAttribArray(vPosition_id);
	//specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
	glVertexAttribPointer(vPosition_id, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));



	GLuint vNormal_id = 1;
	glEnableVertexAttribArray(vNormal_id);
	//note that the pointer offset is not 0, indicating that the Normal data in the vertex array buffer starts right after the color data.
	glVertexAttribPointer(vNormal_id, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numV *sizeof(glm::vec4)));

	GLuint vTextCoord_id = 2;
	glEnableVertexAttribArray(vTextCoord_id);
	//note that the pointer offset is not 0, indicating that the texture data in the vertex array buffer starts right after the normal data.
	glVertexAttribPointer(vTextCoord_id, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numV * 2*sizeof(glm::vec4)));

	GLuint vTangent_id = 3;
	glEnableVertexAttribArray(vTangent_id);
	//note that the pointer offset is not 0, indicating that the texture data in the vertex array buffer starts right after the normal data.
	glVertexAttribPointer(vTangent_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numV * ((2 * sizeof(glm::vec4))+ sizeof(glm::vec2))));

	GLuint vBiTangent_id = 4;
	glEnableVertexAttribArray(vBiTangent_id);
	//note that the pointer offset is not 0, indicating that the texture data in the vertex array buffer starts right after the normal data.
	glVertexAttribPointer(vBiTangent_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(numV * ((2 * sizeof(glm::vec4)) + sizeof(glm::vec2)+ sizeof(glm::vec3))));



	GLuint indexBufferObjectID = 0;
	// Create and bind an Element Buffer Object (EBO)
	glGenBuffers(1, &indexBufferObjectID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObjectID);

	// Allocate and transfer the index data to the GPU
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.geometry.objFaces.size() * sizeof(glm::uvec3), obj.geometry.objFaces.data(), GL_STATIC_DRAW);
	
	glBindVertexArray(0);

	
	


}

void initShaders()
{
	//create, load, compile, attach vertex and fragment shaders
	g_gouraudProgramID = initShader(
		"..\\Shaders\\gouraudVertexShader.glsl",
		"..\\Shaders\\gouraudFragmentShader.glsl"
	);

	g_phongProgramID = initShader(
		"..\\Shaders\\phongVertexShader.glsl",
		"..\\Shaders\\phongFragmentShader.glsl"
	);

	g_textProgramID = initShader(
		"..\\Shaders\\textVertexShader.glsl",
		"..\\Shaders\\textFragmentShader.glsl"
	);

	g_wireFrameProgramID = initShader(
		"..\\Shaders\\wireFrameVertexShader.glsl",
		"..\\Shaders\\wireFrameFragmentShader.glsl"
	);
	g_deformationProgramID = initShader(
		"..\\Shaders\\deformationVertexShader.glsl",
		"..\\Shaders\\phongFragmentShader.glsl"
	);

	g_curveProgramID = initShader(
		"..\\Shaders\\curveVertexShader.glsl",
		"..\\Shaders\\curveFragmentShader.glsl"
	);
	g_programID = 0;
	if (!g_gouraudProgramID || !g_phongProgramID ||
		!g_textProgramID || !g_wireFrameProgramID ||
		!g_deformationProgramID || !g_curveProgramID) {
		std::cout << "\nShader creation failed!\n";
		return;
	}
}

void initBezCurve()
{


	g_bezCurve.sampleCurve(g_numSegs);
	//create a Vertex Array Object (VAO)

	glGenVertexArrays(1, &curveVAOID);
	glBindVertexArray(curveVAOID);

	//create and initialize a Vertex Buffer Object (VBO)
	glGenBuffers(1, &curveVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, curveVBOID);

	
	//this will allocate memory for the buffer object on the GPU
	glBufferData(GL_ARRAY_BUFFER, g_bezCurve.curvePoints.size()*sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, g_bezCurve.curvePoints.size() * sizeof(glm::vec3), g_bezCurve.curvePoints.data());


	GLint vPosition_id = 0;
	//this enables the generic vertex attribute array such that the values in the generic vertex attribute array
	//will be accessed and used for rendering when calls are made to vertex array commands such as glDrawArrays or glDrawElements.
	glEnableVertexAttribArray(vPosition_id);
	//specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
	glVertexAttribPointer(vPosition_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glBindVertexArray(0);


	glGenVertexArrays(1, &controlVAOID);
	glBindVertexArray(controlVAOID);

	//create and initialize a Vertex Buffer Object (VBO)
	glGenBuffers(1, &controlVBOID);
	glBindBuffer(GL_ARRAY_BUFFER, controlVBOID);


	//this will allocate memory for the buffer object on the GPU
	glBufferData(GL_ARRAY_BUFFER, g_bezCurve.controlPoints.size() * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, g_bezCurve.controlPoints.size() * sizeof(glm::vec3), g_bezCurve.controlPoints.data());


	GLint vControlPosition_id = 0;
	//this enables the generic vertex attribute array such that the values in the generic vertex attribute array
	//will be accessed and used for rendering when calls are made to vertex array commands such as glDrawArrays or glDrawElements.
	glEnableVertexAttribArray(vControlPosition_id);
	//specifies an offset of the first component of the first generic vertex attribute in the array in the data store of the buffer currently bound to the GL_ARRAY_BUFFER target
	glVertexAttribPointer(vControlPosition_id, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glBindVertexArray(0);

}




void TW_CALL lookAtObj(void* data) {
	obj.cam.lookAt(obj.position.x, obj.position.y, obj.position.z);
}


