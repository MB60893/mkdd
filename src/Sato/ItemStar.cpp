#include "Sato/ItemStar.h"

#include "JSystem/JAudio/JASFakeMatch2.h"

namespace
{
    f32 sScale = 1.0f;
    f32 sHeightOffset = 26.0f;
    f32 sBoundsRadius = 30.0f;
}

ItemStar::ItemStar(u32 kind) : ItemObj(kind)
{
    reset();
}

void ItemStar::reset()
{
    resetObject();
    mKartReaction = 7;
    _12c |= 8;
    _12c |= 0x200;
    _28 = 0.5f;
    _6c = sHeightOffset;
    _2c = 0.5f;

    if (mBounds)
        mBounds->setRadius(sBoundsRadius);

    mLerpRatio = 0.3f;
    mRot = 0.0f;
    PSMTXIdentity(mRotMtx);
}

const char *ItemStar::getBmdFileName()
{
    static const char *cStarBmdName = "/Item/item_star.bmd";
    return cStarBmdName;
}

void ItemStar::createModel(JKRSolidHeap *heap, u32 p2, u32 id) {
    mModel.createDifferedModel(heap, p2, id | 0x1000200, false);
}

void ItemStar::doUniv()
{
    if (!tstAnyObjectHitting())
    {
        switch (mState)
        {
        case 1:
        case 5:
            doEquipScaleUp(sScale);
            break;
        case 4:
        case 6:
            doFallScaling();
            setScaleCol(mEquipScale);
            break;
        case 7:
            doForceDisappearScaleDown();
            break;
        }
    }
    (this->*mStateFunc)();
}

void ItemStar::initEquip()
{
    mEquipFlags = 0;
    mpHandAnmMtx = getHandAnmMtx();
    mVel.zero();
    mPos.zero();
    JMath::gekko_ps_copy12(mRotMtx, _210); // NOTE: mRotMtx might be SMatrix34C, spot other inlnes to be sure
}

void ItemStar::doEquip() {
    rotationRad(mRot, 0.05f);
    f32 r = mRot;
    f32 s = sin(r);
    f32 c = cos(r);
    Mtx m;

    m[1][1] = 1.0f;
    m[0][0] = c;
    m[0][2] = s;
    m[2][0] = -s;
    m[2][2] = c;
    m[2][1] = 0.0f;
    m[1][2] = 0.0f;
    m[1][0] = 0.0f;
    m[0][1] = 0.0f;
    m[0][3] = 0.0f;
    m[1][3] = 0.0f;
    m[2][3] = 0.0f;
    PSMTXConcat(mRotMtx, m, _210);
    doHandOneEquip(false, 0.0f);
}

void ItemStar::initRelease() { setStateDisappear(true); }

void ItemStar::doRelease() {}

void ItemStar::initDisappear()
{
    _12c &= ~1;
    mState = 10;
}

void ItemStar::doDisappear() {}

void ItemStar::initFall()
{
    mEquipFlags = 0xffffffff;
    ItemObj::initmoveFall();
}

void ItemStar::doFall()
{
    Quaternion qRotY, q;

    rotationRad(mRot, 0.05f);
    QuatRotY(mRot, &qRotY);
    moveFall();
    PSQUATMultiply(&_34, &qRotY, &q);
    setModelMatrixAndScale(q);
}
