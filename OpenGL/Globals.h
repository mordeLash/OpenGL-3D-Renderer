#pragma once
//#include <Obj Parser/wavefront_obj.h>
//#include "sceneLoader.h"
#include "Object.h"
#include "Bezier.h"

typedef  enum { SHAPE_RECTANGLE = 1, SHAPE_TRIANGLE, SHAPE_STAR, SHAPE_HOUSE, SHAPE_STAR_OF_DAVID, SHAPE_CIRCLE, SHAPE_LINE } Shape;
#define NUM_SHAPES 7

//globals HW3
extern float g_OKa;
extern float g_OKd;
extern float g_OKs;
extern int g_OSc;

extern unsigned int g_OColor;
extern bool g_DSCB;

typedef  enum { DIRECTIONAL = 1, POINT_LIGHT, AMBIENT, OFF } LightType;
#define NUM_LIGHT_TYPES 2
extern LightType g_L1T;
extern double g_L1D[];
extern float g_L1I[];
extern double g_L1P[];

extern bool g_L2CB;
extern LightType g_L2T;
extern double g_L2D[];
extern float g_L2I[];
extern double g_L2P[];
extern float g_LAMI[];

typedef  enum { NONE = 1, GOURAUD, PHONG,TEXTURE,DEFORMATION } ShadingType;
#define NUM_SHADING_TYPES 5
extern ShadingType g_SType;



typedef  enum { SLERP = 1, EULER} interType;
#define NUM_INTER_TYPES 2
extern interType g_IType;

typedef enum { FOV = 1, FRUSTRUM } camType;
#define NUM_CAM_TYPES 2
extern camType g_cType;

typedef enum { FULL = 1, HALF } defType;
#define NUM_DEF_TYPES 2
extern defType g_dType;


typedef enum { STATIC = 1, ANIMATION } displayType;
#define NUM_DIS_TYPES 2
extern displayType g_disType;

typedef enum { SNAP_TO_CURVE = 1, SNAP_TO_OBJECT } curveType;
#define NUM_CURVE_TYPES 2
extern curveType g_curveType;

typedef enum { QUATERNIONS = 1, ANGLES } rotationType;
#define NUM_ROT_TYPES 2
extern rotationType g_rotType;




//define globals
extern float g_Fn, g_Ff, g_Fr, g_Ft;
extern int g_CamM;
extern float g_Fov, g_Cx, g_Cy, g_Cz;
extern float g_Tx, g_Ty, g_Tz;

extern float g_Rx, g_Ry, g_Rz;

extern float g_Os;

extern int g_width, g_height;
extern Object obj;
extern Shape g_Shape;

extern float g_scale;
extern float g_quaternion[4];

extern float g_startQ[4];
extern float g_endQ[4];


//points coordinates
extern int g_P1x, g_P1y; //P1
extern int g_P2x, g_P2y; //P2
extern int g_P3x, g_P3y; //P3
extern unsigned int g_Color;
extern float g_Fn, g_Ff, g_Fr, g_Ft;

extern float g_lineW;

extern float g_time;
extern float g_speed;
extern float g_maxD;
extern float g_DT;


extern Bezier g_bezCurve;
extern unsigned int g_numSegs;
extern bool g_play;
extern bool g_displayCurve;
extern bool g_snap;

extern bool g_texturise;
extern bool g_map;





extern glm::vec3 g_SR;
extern glm::vec3 g_ER;