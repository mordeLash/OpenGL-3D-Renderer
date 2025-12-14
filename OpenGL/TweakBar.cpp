#include "TweakBar.h"

void TW_CALL transObj(void* clientData)
{
	obj.modelingMat = getTranslation({ g_Tx,g_Ty, g_Tz, }) * obj.modelingMat;
	obj.calcModelViewMat();
}

void TW_CALL transWorld(void* clientData)
{
	obj.viewMat = getTranslation({ g_Tx,g_Ty, g_Tz, }) * obj.viewMat;
	obj.calcModelViewMat();


}

void TW_CALL ScaleObj(void* clientData)
{
	obj.modelingMat = getScale({ g_Os,g_Os, g_Os }) * obj.modelingMat;
	obj.calcModelViewMat();

}

void TW_CALL ScaleWorld(void* clientData)
{
	obj.viewMat = getScale({ g_Os,g_Os, g_Os }) * obj.viewMat;
	obj.calcModelViewMat();

}

void TW_CALL RotZObj(void* clientData)
{
	obj.modelingMat = getRotation(g_Rz, 0) * obj.modelingMat;
	obj.calcModelViewMat();

}

void TW_CALL RotZWorld(void* clientData)
{
	obj.viewMat = getRotation(g_Rz, 0) * obj.viewMat;
	obj.calcModelViewMat();

}

void TW_CALL RotYObj(void* clientData)
{
	obj.modelingMat = getRotation(g_Ry, 1) * obj.modelingMat;
	obj.calcModelViewMat();

}

void TW_CALL RotYWorld(void* clientData)
{
	obj.viewMat = getRotation(g_Ry, 1) * obj.viewMat;
	obj.calcModelViewMat();

}

void TW_CALL RotXObj(void* clientData)
{
	obj.modelingMat = getRotation(g_Rx, 2) * obj.modelingMat;
	obj.calcModelViewMat();


}

void TW_CALL RotXWorld(void* clientData)
{
	obj.viewMat = getRotation(g_Rx, 2) * obj.viewMat;
	obj.calcModelViewMat();

}


void objectLoadedOptions()
{
	TwDefine("TweakBar/'Rotate Object' visible=true opened=false");
	TwDefine("TweakBar/'Scale Object' visible=true opened=false");
	TwDefine("TweakBar/'Translate Object' visible=true opened=false");
	TwDefine("TweakBar/'Frustrom' visible=true opened=false");
	TwDefine("TweakBar/'Cam Position' visible=true opened=false");
	TwDefine("TweakBar/'Object Properties' visible=true opened=false");
	TwDefine("TweakBar/'Lights' visible=true opened=false");
	TwDefine("TweakBar/'Animation' visible=true opened=false");
	TwDefine("TweakBar/'Bezier Curve' visible=true opened=false");
	TwDefine("TweakBar/'Shading' visible=true opened=false");
	TwDefine("TweakBar/'Display type' visible=true ");
}

void startUpOptions()
{

	TwDefine("TweakBar/'Rotate Object' visible=false");
	TwDefine("TweakBar/'Scale Object' visible=false");
	TwDefine("TweakBar/'Translate Object' visible=false");
	TwDefine("TweakBar/'Frustrom' visible=false");
	TwDefine("TweakBar/'Cam Position' visible=false");
	TwDefine("TweakBar/'Object Properties' visible=false");
	TwDefine("TweakBar/'Lights' visible=false");
	TwDefine("TweakBar/'Animation' visible=false");
	TwDefine("TweakBar/'Bezier Curve' visible=false");
	TwDefine("TweakBar/'Shading' visible=false");
	TwDefine("TweakBar/'Display type' visible=false");
}
