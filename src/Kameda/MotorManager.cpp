#include "Kameda/MotorManager.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Kameda/SequenceInfo.h"
#include "Kaneshige/RaceMgr.h"
#include "Osako/kartPad.h"
#include "mathHelper.h"  // For unused data

int MotorManager::mCountFrame[8][MotorManager::MotorType_MAX];

MotorManager *MotorManager::mThis;

MotorManager::MotorManager(JKRHeap *heap)
{
    if (!heap)
    {
        heap = JKRHeap::getCurrentHeap();
    }

    mThis = this;

    mMotor = new (heap, 0) Motor(heap);
    mKartNumber = RaceMgr::getManager()->getKartNumber();
}

MotorManager::~MotorManager()
{
    delete mMotor;
}

void MotorManager::reset()
{
    mMotor->init();

    for (s32 i = 0; i < mKartNumber; ++i)
    {
        for (s32 j = 0; j < MotorType_MAX; ++j)
        {
            mCountFrame[i][j] = 0;
        }
    }
}

void MotorManager::exec()
{
    mMotor->exec();

    for (s32 i = 0; i < mKartNumber; ++i)
    {
        for (s32 j = 0; j < MotorType_MAX; ++j)
        {
            mCountFrame[i][j] += 1;
        }
    }
}

void MotorManager::setMotor(MotorManager::MotorType motorType, int kart)
{
#line 117
    JUT_MINMAX_ASSERT(0, motorType, MotorType_MAX);
    JUT_MINMAX_ASSERT(0, kart, 8);

    RaceMgr *raceMgr = RaceMgr::getManager();
    if (!raceMgr->isRaceEnd() && !raceMgr->getKartChecker(kart)->isDead())
    {
        if (mCountFrame[kart][motorType] >= 10)
        {
            mCountFrame[kart][motorType] = 0;

            KartGamePad *kartGamePad = KartCtrl::getKartCtrl()->getPad(kart);
            KartGamePad *kartGamePadCo = KartCtrl::getKartCtrl()->getCoPad(kart);

            s32 patternIndex = motorType + 1;
            mThis->mMotor->triggerWave(kartGamePad, patternIndex);

            if ((KartCtrl::getKartCtrl()->GetGameStatus(kart) & 1) != 0)
            {
                mThis->mMotor->triggerWave(kartGamePadCo, patternIndex);
            }
        }
    }
}

void MotorManager::setAwardMotor(MotorManager::MotorType motorType)
{
#line 157
    JUT_MINMAX_ASSERT(0, motorType, MotorType_MAX);

    s32 patternIndex = motorType + 1;
    for (s32 i = 0; i < gSequenceInfo._350; ++i)
    {
        mThis->mMotor->triggerWave(gpaKartPad[i], patternIndex);
    }
}

void MotorManager::setPowerMotor(int kart, f32 strength, u8 phase1Duration, u8 phase2Duration)
{
#line 176
    JUT_MINMAX_ASSERT(0, kart, 8);

    RaceMgr *raceMgr = RaceMgr::getManager();
    if (!raceMgr->isRaceEnd() && !raceMgr->getKartChecker(kart)->isDead())
    {
        KartGamePad *kartGamePad = KartCtrl::getKartCtrl()->getPad(kart);
        KartGamePad *kartGamePadCo = KartCtrl::getKartCtrl()->getCoPad(kart);

        mThis->mMotor->powerWave(kartGamePad, strength, phase1Duration, phase2Duration);

        if ((KartCtrl::getKartCtrl()->GetGameStatus(kart) & 1) != 0)
        {
            mThis->mMotor->powerWave(kartGamePadCo, strength, phase1Duration, phase2Duration);
        }
    }
}

#include "JSystem/JAudio/JASFakeMatch2.h"  // For static initializer
