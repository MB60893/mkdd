#include "Kameda/K2DPicture.h"

#include "JSystem/JUtility/TColor.h"

#include "dolphin/gx/GXEnum.h"
#include "dolphin/gx/GXGeometry.h"
#include "dolphin/gx/GXLighting.h"
#include "dolphin/gx/GXPixel.h"
#include "dolphin/gx/GXStruct.h"
#include "dolphin/gx/GXTev.h"
#include "dolphin/mtx.h"

static const f32 sZeroDegreeAngle = 0.f;
static const f32 sDegToRad = 0.017453292f;

u8 K2DPicture::mK2DGlobalAlpha = 0xFF;
u8 K2DPicture::mDummy[4] = {0, 0, 0, 0};

void K2DPicture::drawK2D(float x0, float y0, float x1, float y1, bool tevMode) {

    if(!mPicture.mIsVisible)
    {
        return;
    }

    for(u8 i = 0; i < mPicture.mTextureCount; i++)
    {
        mPicture.load(i);
    }

    GXSetNumTexGens(mPicture.mTextureCount);

    mPicture.mColorAlpha = mPicture.mAlpha;

    JUTColor newColor[4] = {-1, -1, -1, -1};

    getNewColor(newColor);

    if(tevMode) {
        setTevModeK2D();
    }
    else {
        if(mPicture.mTextureCount != 1)
        {
            GXSetTevKColor(GX_KCOLOR0, mPicture.mBlendColor);
            GXSetTevKColor(GX_KCOLOR2, mPicture.mBlendAlpha);
        }

        if (mPicture.mBlack != 0 || mPicture.mWhite != -1) {
            GXSetTevColor(GX_TEVREG0, mPicture.mBlack);
            GXSetTevColor(GX_TEVREG1, mPicture.mWhite);
        }
    }

    makeMatrix(x0, y0);
    GXLoadPosMtxImm(mPicture.mPositionMtx, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    const u8 maxCoord = 0x80;

    GXPosition2f32(sZeroDegreeAngle, sZeroDegreeAngle);
    GXColor1u32(newColor[0]);
    GXTexCoord2u8(0, 0);

    GXPosition2f32(x1, sZeroDegreeAngle);
    GXColor1u32(newColor[1]);
    GXTexCoord2u8(maxCoord, 0);

    GXPosition2f32(x1, y1);
    GXColor1u32(newColor[3]);
    GXTexCoord2u8(maxCoord, maxCoord);

    GXPosition2f32(sZeroDegreeAngle, y1);
    GXColor1u32(newColor[2]);
    GXTexCoord2u8(0, maxCoord);

    GXEnd();
}

void K2DPicture::getNewColor(JUTColor colors[4]) {
    colors[0].r = mPicture.mCornerColors[0].r;
    colors[0].g = mPicture.mCornerColors[0].g;
    colors[0].b = mPicture.mCornerColors[0].b;
    colors[0].a = mPicture.mCornerColors[0].a;

    colors[1].r = mPicture.mCornerColors[1].r;
    colors[1].g = mPicture.mCornerColors[1].g;
    colors[1].b = mPicture.mCornerColors[1].b;
    colors[1].a = mPicture.mCornerColors[1].a;

    colors[2].r = mPicture.mCornerColors[2].r;
    colors[2].g = mPicture.mCornerColors[2].g;
    colors[2].b = mPicture.mCornerColors[2].b;
    colors[2].a = mPicture.mCornerColors[2].a;

    colors[3].r = mPicture.mCornerColors[3].r;
    colors[3].g = mPicture.mCornerColors[3].g;
    colors[3].b = mPicture.mCornerColors[3].b;
    colors[3].a = mPicture.mCornerColors[3].a;

    if(mPicture.mColorAlpha != 0xFF)
    {
        colors[0].a = (colors[0].a * mPicture.mColorAlpha / 0xFF);
        colors[1].a = (colors[1].a * mPicture.mColorAlpha / 0xFF);
        colors[2].a = (colors[2].a * mPicture.mColorAlpha / 0xFF);
        colors[3].a = (colors[3].a * mPicture.mColorAlpha / 0xFF);
    }

    if(mK2DGlobalAlpha == 0xFF)
    {
        return;
    }

    colors[0].a = ((colors[0].a * mK2DGlobalAlpha) / 0xFF);
    colors[1].a = ((colors[1].a * mK2DGlobalAlpha) / 0xFF);
    colors[2].a = ((colors[2].a * mK2DGlobalAlpha) / 0xFF);
    colors[3].a = ((colors[3].a * mK2DGlobalAlpha) / 0xFF);
}

void K2DPicture::setTevModeK2D() {
    bool useBlackAndWhite = false;
    bool useColorBlend = false;
    u32 numStages;

    if(mPicture.mBlack != 0 || mPicture.mWhite != -1)
    {
        useBlackAndWhite = true;
    }

    if (mPicture.mColorAlpha != 0xff ||
        mPicture.mCornerColors[0] != -1  ||
        mPicture.mCornerColors[1] != -1  ||
        mPicture.mCornerColors[2] != -1  ||
        mPicture.mCornerColors[3] != -1  ||
        K2DPicture::mK2DGlobalAlpha != 0xff)
    {
        useColorBlend = true;
    }

    if(!useColorBlend) {
        if(!useBlackAndWhite){
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);

            if ((s32)mPicture.mTextures[0]->getTexInfo()->mTransparency != 0) {
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
            } else {
                GXSetTevColor(GX_TEVREG2, JUtility::TColor(-1));
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
            }

            numStages = 1;
        }
        else {
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

            GXSetTevColor(GX_TEVREG0, mPicture.mBlack);
            GXSetTevColor(GX_TEVREG1, mPicture.mWhite);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);

            if ((s32)mPicture.mTextures[0]->getTexInfo()->mTransparency != 0) {
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
            } else {
                GXSetTevColor(GX_TEVREG2, JUtility::TColor(-1));
                GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_A2, GX_CA_ZERO);
            }

            numStages = 1;
        }
    } else if (!useBlackAndWhite) {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);

        if ((s32)mPicture.mTextures[0]->getTexInfo()->mTransparency != 0) {
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        } else {
            GXSetTevColor(GX_TEVREG2, JUtility::TColor(-1));
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A0, GX_CA_RASA, GX_CA_ZERO);
        }

        numStages = 1;
    } else {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);

        GXSetTevColor(GX_TEVREG0, *(GXColor*)&mPicture.mBlack);
        GXSetTevColor(GX_TEVREG1, *(GXColor*)&mPicture.mWhite);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);

        if ((s32)mPicture.mTextures[0]->getTexInfo()->mTransparency != 0) {
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
        } else {
            GXSetTevColor(GX_TEVREG2, JUtility::TColor(-1));
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_A2, GX_CA_ZERO);
        }

        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);

        numStages = 2;
    }

    GXSetNumTevStages(numStages);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetNumChans(1);
}

void K2DPicture::makeMatrix(f32 x, f32 y) {
    Mtx anchorToOriginMatrix;
    Mtx rotationMatrix;
    Mtx originToPosMatrix;

    if (mPicture.mAngleZ != sZeroDegreeAngle) {
        PSMTXTrans(anchorToOriginMatrix, -mPicture.mAnchorPoint.x, -mPicture.mAnchorPoint.y, sZeroDegreeAngle);

        f32 degree;
        if (mPicture.mRotationAxis == J2DROTATE_Z) {
            degree = -mPicture.mAngleZ;
        } else {
            degree = mPicture.mAngleZ;
        }

        PSMTXRotRad(rotationMatrix, mPicture.mRotationAxis, degree * sDegToRad);

        PSMTXTrans(originToPosMatrix, mPicture.mAnchorPoint.x + x, mPicture.mAnchorPoint.y + y, sZeroDegreeAngle);

        PSMTXConcat(rotationMatrix, anchorToOriginMatrix, mPicture.mPositionMtx);

        PSMTXConcat(originToPosMatrix, mPicture.mPositionMtx, mPicture.mPositionMtx);
    } else {
        PSMTXTrans(mPicture.mPositionMtx, x, y, sZeroDegreeAngle);
    }
}
