#ifndef MAPOBJDOSSUN_H
#define MAPOBJDOSSUN_H

#include "JSystem/J3D/J3DAnmBase.h"
#include "JSystem/J3D/J3DModel.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "Kaneshige/Course/CrsData.h"
#include "Kaneshige/ExModel.h"
#include "Sato/AnmController.h"
#include "Sato/StateObserver.h"
#include "Shiraiwa/Coord3D.h"

class TMapObjDossun {
public:
    TMapObjDossun(const CrsData::SObject &);
    ~TMapObjDossun();
    virtual void loadAnimation();
    virtual void createModel(JKRSolidHeap *, u32);
    virtual void reset();
    virtual void calc();
    virtual void update();
    virtual const char *getBmdFileName();
    virtual const char *getShadowBmdFileName();
    virtual void createColModel(J3DModelData *);
    
    void initFunc_Start();
    void doFunc_Start();
    void initFunc_Up();
    void doFunc_Up();
    void initFunc_Top();
    void doFunc_Top();
    void initFunc_Down();
    void doFunc_Down();
    void makeGroundEmitter();
    void initFunc_Bottom();
    void doFunc_Bottom();
    void initFunc_Die();
    void doFunc_Die();
    void shakeCamera();
    void checkItemHitting();
    
    virtual void doKartColCallBack(int param_1);
    virtual u32 getReleaseFrame();
    virtual void InitExec();
    virtual void MoveExec();

    static J3DAnmTransform *sDossunBckAnmTrans;
    static J3DMtxCalc *sDossunBckMtxCalc;
    static J3DAnmBase *sDossunBtpAnm;

private:
    JGeometry::TVec3f _4;               // 0x4   - Camera pos?
    u8 _10[0x40 - 0x10];
    JGeometry::TVec3f _40;              // 0x40
    JGeometry::TVec3f _4c;              // 0x4c
    u8 _58[0x5c - 0x58];
    ExModel _5c;                        // 0x5c
    s32 _e8;                            // 0xe8 - is this an undefined class object?
    u32 someFlags;                      // 0xec
    u8 _f0[0xfc - 0xf0];                // Possible: u32 f0 (mask or flags?)
    JSUPtrLink _fc;                     // 0xfc
    u8 _10c[0x120 - 0x10c];
    AnmController *mAnmController;      // 0x120
    u8 _124[0x174 - 0x124];
    StateObserver *_174;                // 0x174 - Might be something else... abstract class suggests could be StateObserverT<> instead?
    u8 _178[8];
    J3DAnmObjBase *_180;                // 0x180
    ExModel *_184;                      // 0x184
    J3DFrameCtrl *_188;                 // 0x188
    u8 _18c[16];
    J3DAnmBase *_19c;                   // 0x19c
    s16 _1a0;                           // 0x1a0
    u16 _1a2;                           // 0x1a2
    u8 _1a4;                            // 0x1a4
    u8 _1a5;                            // 0x1a5 - Padding...?
    u16 _1a6;                           // 0x1a6
    TFreeMove mFreeMove;                // 0x1a8
    u8 _1ac[0x1c0 - 0x1a8];
    f32 _1c0;                           // 0x1c0
    u8 _1c4[4];
    f32 _1cc;                           // 0x1cc
    TPathMove *mPathMove;               // 0x1d0
    s16 _1d4;                           // 0x1d4
}; // 0x1d8

#endif // MAPOBJDOSSUN_H
