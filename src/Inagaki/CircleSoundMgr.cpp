
#include "Inagaki/GameAudioHostIO.h"
#include "Inagaki/GameAudioMain.h"
#include "Inagaki/GameSoundMgr.h"
#include "Inagaki/GameAudioCamera.h"

#include "JSystem/JAudio/Interface/JAIAudience.h"
#include "JSystem/JAudio/Interface/JAISound.h"
#include "JSystem/JAudio/JASFakeMatch2.h"
#include "JSystem/JAudio/System/JASGadget.h"
#include "JSystem/JGeometry/Vec.h"
#include "JSystem/JUtility/JUTAssert.h"

#include "dolphin/mtx.h"

#include "std/math.h"

namespace GameAudio {

CircleSoundMgr::CircleSoundMgr(Vec *pos, f32 radius, f32 minY, f32 maxY, JKRHeap *heap)
    : SoundMgr<CircleSoundMgr>(pos, heap, 4) {
    mRadius = radius;
    mMinY = minY;
    mMaxY = maxY;

    if(mMinY == mMaxY)
    {
        mHasHeightRange = false;
    }
    else {
        mHasHeightRange = true;
    }

    mNoClamp = false;
    // Unused
    _5c = false;

    mPlayPos = new JGeometry::TVec3f[4];
    mPlayPosComputed = new bool[4];

    for(u8 i = 0; i < 4; i++)
    {
        mPlayPos[i].set(0.f, 0.f, 0.f);
        mPlayPosComputed[i] = false;
    }
}

CircleSoundMgr::~CircleSoundMgr() {}

void CircleSoundMgr::loop() {
    #line 63
    JUT_ASSERT_MSG(mSoundPos != NULL, "CircleSoundMgr::loop　disable中です！！！！！\n");

    if(mKillSw) {
        return;
    }
    
    const u32 sceneMax = GameAudio::Main::getAudio()->getCamera()->getSceneMax();

    // Forced to cast sceneMax into its own u32 type here...
    for(s32 index = 0; index < (u32)sceneMax; index++){
        if ((*this)[index] == NULL) {
            continue;
        }

        mPlayPosComputed[index] = calcPlayPos(index);

        const JGeometry::TVec3f& playPos = getPlayPos(index);
        JAIAudible *audible = (*this)[index]->getAudible();
        if(audible) {
            audible->setPos(playPos);
        }
    }
}

void CircleSoundMgr::frameWork() {
    if(mKillSw) {
        return;
    }
    loop();
}

bool CircleSoundMgr::calcPlayPos(u32 index) {
    mPlayPosComputed[index] = false;

    CameraInfo* info = GameAudio::Main::getAudio()->getCamera()->getCameraInfo(index);

    JGeometry::TVec3<f32> cameraPosition (info->_0);

    f32 distanceZ = mSoundPos->z - cameraPosition.z;
    f32 distanceX = mSoundPos->x - cameraPosition.x;
    f32 distanceSquare = (distanceX * distanceX) + (distanceZ * distanceZ);

    f32 radiusSquare = mRadius * mRadius;

    if(radiusSquare > distanceSquare)
    {
        mPlayPos[index] = cameraPosition;
    }
    else {
        if (mNoClamp == false) {
            f32 distance = std::sqrtf(distanceSquare);

            f32 scale = mRadius / distance;

            JGeometry::TVec3<f32> toCamera = (cameraPosition - *mSoundPos);
            mPlayPos[index] = (toCamera * scale) + *mSoundPos;
        }
        else {
            return false;
        }
    }

    if(mHasHeightRange == 0) // 2c8
    {
        mPlayPos[index].y = cameraPosition.y;
    }
    else if (cameraPosition.y < mMinY) {
        if (mNoClamp == false) {
            mPlayPos[index].y = mMinY;
        }
        else {
            return false;
        }
    }
    else if (cameraPosition.y > mMaxY) {
        if (mNoClamp == false) {
            mPlayPos[index].y = mMaxY;
        }
        else {
            return false;
        }
    }
    else {
        mPlayPos[index].y = cameraPosition.y;
    }

    mPlayPosComputed[index] = true;
    return true;
}

void CircleSoundMgr::setSe(u32 soundEffectID) {
    if(mKillSw) {
        return;
    }

    JAISoundStarter* soundStarter = JASGlobalInstance<JAISoundStarter>::getInstance();
    const u32 sceneMax = GameAudio::Main::getAudio()->getCamera()->getSceneMax();

    for(s32 index = 0; index < (u32)sceneMax; index++) {
        if(!calcPlayPos(index)) {
            continue;
        }

        JAISoundHandle& handle = (*this)[index];

        soundStarter->startSound(soundEffectID, &handle, NULL);

        if(handle.sound_ == NULL) {
            continue;
        }

        if(handle->getAudible() == NULL) {
            const u32 otherScreensMask = (1 << index) ^ 0xf;

            handle->newAudible(
                JGeometry::TVec3f (mPlayPos[index]),
                &mPlayPos[index],
                otherScreensMask,
                NULL);
        }

        const s32 partnerCameraIndex = index ^ 2;
        if(partnerCameraIndex >= sceneMax) {
            continue;
        }

        const JGeometry::TVec3f partnerCameraPosition
            = (GameAudio::Main::getAudio()->getCamera()->getCameraInfo(partnerCameraIndex)->_0);
        
        const JGeometry::TVec3f& cameraPosition
            = (GameAudio::Main::getAudio()->getCamera()->getCameraInfo(index)->_0);

        const f32 distance = PSVECDistance(&partnerCameraPosition, &cameraPosition);

        if(distance < GameAudio::GA_MULTI_CAMERA_VOL_DOWN_DISTANCE)
        {
            const f32 volume = 1.f - (
                (GameAudio::GA_MULTI_CAMERA_VOL_DOWN_RATE * (GA_MULTI_CAMERA_VOL_DOWN_DISTANCE - distance))
                    / GA_MULTI_CAMERA_VOL_DOWN_DISTANCE);

            handle->params_.mMove.moveVolume(volume, 0);
        }
    }
}

}
