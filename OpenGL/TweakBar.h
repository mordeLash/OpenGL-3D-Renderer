#pragma once
#include <AntTweakBar/include/AntTweakBar.h>
#include "Globals.h"

//function that deal with tweak bar


void TW_CALL transObj(void* clientData);
void TW_CALL transWorld(void* clientData);
void TW_CALL transObj(void* clientData);
void TW_CALL transWorld(void* clientData);

void TW_CALL ScaleObj(void* clientData);
void TW_CALL ScaleWorld(void* clientData);

void TW_CALL RotZObj(void* clientData);
void TW_CALL RotZWorld(void* clientData);

void TW_CALL RotYObj(void* clientData);
void TW_CALL RotYWorld(void* clientData);

void TW_CALL RotXObj(void* clientData);
void TW_CALL RotXWorld(void* clientData);


void TW_CALL loadOBJModel(void* clientData);
void TW_CALL lookAtObj(void* clientData);

void objectLoadedOptions();

void startUpOptions();

