#include "Shiraiwa/MapObjDossun.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Sato/ObjUtility.h"


void TMapObjDossun::reset() {}

void TMapObjDossun::loadAnimation() {}

const char *TMapObjDossun::getShadowBmdFileName() {
    static const char *cShadowBmdName = "/Objects/Dossun1_kage.bmd";
    return cShadowBmdName;
}

void TMapObjDossun::createColModel(J3DModelData *j3dModelData) {}

void TMapObjDossun::createModel(JKRSolidHeap *heap, u32 viewNo) {}

const char *TMapObjDossun::getBmdFileName() {
    static const char *cBmdName = "/Objects/Dossun1.bmd";
    return cBmdName;
}

void TMapObjDossun::update() {}

void TMapObjDossun::doKartColCallBack(int) {}

void TMapObjDossun::initFunc_Start() { return; }

void TMapObjDossun::doFunc_Start() {}

void TMapObjDossun::initFunc_Up() {}

void TMapObjDossun::doFunc_Up() {}

void TMapObjDossun::initFunc_Top() {}

void TMapObjDossun::doFunc_Top() {}

void TMapObjDossun::initFunc_Down() {}

void TMapObjDossun::doFunc_Down() {}

void TMapObjDossun::makeGroundEmitter() {}

void TMapObjDossun::initFunc_Bottom() {}

void TMapObjDossun::doFunc_Bottom() {}

void TMapObjDossun::initFunc_Die() {}

void TMapObjDossun::doFunc_Die() {}

void TMapObjDossun::shakeCamera() {
    ObjUtility::executeShakeCamera(_4, 10000.0f, 0.4f, 2000.0f);
}

void TMapObjDossun::calc() {}

u32 TMapObjDossun::getReleaseFrame() {
    return 100;
}

void TMapObjDossun::checkItemHitting() {}

void TMapObjDossun::InitExec() {}

void TMapObjDossun::MoveExec() {}
