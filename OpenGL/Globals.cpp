#include "Globals.h"


float g_OKa = 0.5f;
float g_OKd = 0.3f;
float g_OKs = 0.2f;
int g_OSc = 2;
unsigned int g_OColor = 0xffffffff;
bool g_DSCB;

LightType g_L1T = DIRECTIONAL;
double g_L1D[] = { 0.0, 0.0, -1.0 };
float g_L1I[] = { 1.0, 1.0, 1.0 };
double g_L1P[] = { 0,0,1000 };

bool g_L2CB;
LightType g_L2T = DIRECTIONAL;
double g_L2D[] = { 0.0, 0.0, -1.0 };
float g_L2I[] = { 1.0,1.0, 1.0 };
double g_L2P[] = { 0,0,1000 };

float g_LAMI[] = { 1.0,1.0, 1.0 };

ShadingType g_SType = TEXTURE;
interType g_IType = SLERP;
camType g_cType = FOV;
defType g_dType = FULL;

bool g_NoCB = false;
bool g_BBCB = false;
bool g_WC = false;
bool g_OC = false;
int g_CamM = 0;
float g_NoS = 1.0f;
float g_Fov = 45.0f;
float g_Cx = 0.0f;
float g_Cy = 0.0f;
float g_Cz = 0.0f;

float g_Tx = 0.0f;
float g_Ty = 0.0f;
float g_Tz = 0.0f;

float g_Rx = 0.0f;
float g_Ry = 0.0f;
float g_Rz = 0.0f;

float g_Os = 1.0f;

int g_width = 940;
int g_height = 640;
//obj data type
Object obj;


float g_quaternion[4] = { 0.0, 0.0, 0.0, 1.0 };
float g_startQ[4] = { 0.0, 0.0, 0.0, 1.0 };
float g_endQ[4] = { 0.0, 0.0, 0.0, 1.0 };
float g_scale = 1.0f;
float g_Fn = 1, g_Ff = 1000, g_Fr = 0.5, g_Ft = 0.5;
unsigned int g_Color = 0xffffffff;

float g_time = 0.0f;
float g_speed = 1;
float g_maxD = 1;
float g_DT = 3;

displayType g_disType = STATIC;
curveType g_curveType = SNAP_TO_CURVE;
rotationType g_rotType = QUATERNIONS;


Bezier g_bezCurve = Bezier({ glm::vec3((-10.0f), (-4.5f), (-15.0f)),
		glm::vec3((-8.0f), (0.0f), (-15.0f)),
		glm::vec3((8.5f), (6.5f), (-15.0f)),
		glm::vec3((10.0f), (3.0f), (-15.0f)) });
unsigned int g_numSegs = 100;
bool g_play = false;
bool g_displayCurve = false;
float g_lineW = 1.0f;
bool g_snap = true;

bool g_texturise = false;
bool g_map = false;

glm::vec3 g_SR = { 0.0f,0.0f,0.0f };
glm::vec3 g_ER = {0.0f,0.0f,0.0f};