#include "Kameda/Motor.h"

#include "Kameda/PauseManager.h"
#include "Kaneshige/RaceMgr.h"
#include "Osako/SystemRecord.h"
#include "Osako/kartPad.h"
#include "Yamamoto/kartCtrl.h"
#include "dolphin/pad.h"

MotorParam::MotorParam()
{
    mUnknownContainer[0].flag = false;
    mUnknownContainer[1].flag = false;
    mUnknownContainer[2].flag = false;
    mUnknownContainer[3].flag = false;
    mUnknownFloat = 1.0f;
    mUnknownValue = 0x1e;
    mUnknownFlag = false;
}

// clang-format off
const char *Motor::mPatternWave[26] = {
    "/",
    "*****---**--*",
    "*******---**-*",
    "*********----**-*",
    "**-*********----***--**",
    "*********/--**",
    "*****-*--*---*----*-----*",
    "****-*--*---*----*",
    "****--**",
    "***********-**-*-*--*---*----*-----*-------*----------*",
    "******---***/----**/----*----*",
    "***********/",
    "*****************/---***/---**/---*/",
    "********-----**",
    "**********---***---***---***---***---***---**---**---*---*---*-------*",
    "******--**---**",
    "*-**-----/**********--/**",
    "*******/---*----*------*----------*",
    "*******----------*",
    "**************************/-******/---***/----**/----*------------*",
    "********/",
    "**------*-----*---*--*-*-******-*-*--*--*---*----*-----*------*-------*--------*---------*----------*",
    "***-------*-------*------*-----*----*----*---*---*---*---*---*--*--*--*-*-*-*-*-*-*-*-*-*-*-*--*--*--*--*---*---*---*---*---*---*---*---*----*----*-----*-----*-----*------*------*------*-------*-------*-------*-------*--------*----------*------------*",
    "******---*-------*/",
    "******-***-**-*--*------*",
    "*****--**--*",
};
// clang-format on

Motor::Motor(JKRHeap *heap)
{
    mHioNode = new (heap, 0) MotorHioNode();

    init();
}

void Motor::init()
{
    for (int i = 0; i < KartGamePad::PORT_COUNT; ++i)
    {
        mMotorData[i].mKartGamePad = nullptr;

        for (int j = 0; j < 4; ++j)
        {
            mMotorData[i].mDriverMotor[j].pattern = mPatternWave[0];
            mMotorData[i].mDriverMotor[j].pos = -1;

            mMotorData[i].mUnknown[j] = 0;

            mMotorData[i].mPartnerMotor[j].pattern = mPatternWave[0];
            mMotorData[i].mPartnerMotor[j].pos = -1;
        }
    }

    mUnknown = 0;
    mSomeCount = 0;

    stop();
}

Motor::~Motor() { delete mHioNode; }

void Motor::exec()
{
    if (!(gSystemRecord.isRumble() && !PauseManager::tstPause() &&
          !RaceMgr::getManager()->isRaceEnd() &&
          RaceMgr::getManager()->getRacePhase() != PHASE_CRS_DEMO &&
          !RaceMgr::getManager()->isReplayMode() && !RaceMgr::getManager()->isWaitDemoMode()))
    {
        stop();
        return;
    }

    for (int i = 0; i < KartGamePad::PORT_COUNT; ++i)
    {
        KartGamePad *kartGamePad = mMotorData[i].mKartGamePad;
        if (!kartGamePad)
            continue;

        int patternValue = '/';

        for (int j = 0; j < 4; ++j)
        {
            s32 pos = mMotorData[i].mDriverMotor[j].pos;
            if (pos >= 0 && mMotorData[i].mUnknown[j])
            {
                if (mMotorData[i].mDriverMotor[j].pattern[pos] == '0')
                {
                    mMotorData[i].mDriverMotor[j].pos = pos + 1;
                }

                if (mMotorData[i].mDriverMotor[j].pattern[mMotorData[i].mDriverMotor[j].pos] == '1')
                {
                    for (pos = 0; mMotorData[i].mDriverMotor[j].pattern[pos] != '\0'; ++pos)
                    {
                        if (mMotorData[i].mDriverMotor[j].pattern[pos] == '0')
                            break;
                    }
                    mMotorData[i].mDriverMotor[j].pos = pos + 1;
                }

                if (mMotorData[i].mDriverMotor[j].pattern[mMotorData[i].mDriverMotor[j].pos] ==
                    '\0')
                {
                    mMotorData[i].mDriverMotor[j].pos = 0;
                }
            }

            pos = mMotorData[i].mPartnerMotor[j].pos;
            if (pos >= 0)
            {
                if (mMotorData[i].mPartnerMotor[j].pattern[pos] == '0')
                {
                    mMotorData[i].mPartnerMotor[j].pos = pos + 1;
                }

                if (mMotorData[i].mPartnerMotor[j].pattern[mMotorData[i].mPartnerMotor[j].pos] ==
                    '1')
                {
                    for (pos = 0; mMotorData[i].mPartnerMotor[j].pattern[pos] != '\0'; ++pos)
                    {
                        if (mMotorData[i].mPartnerMotor[j].pattern[pos] == '0')
                            break;
                    }
                    mMotorData[i].mPartnerMotor[j].pos = pos + 1;
                }

                if (mMotorData[i].mPartnerMotor[j].pattern[mMotorData[i].mPartnerMotor[j].pos] ==
                    '\0')
                {
                    mMotorData[i].mPartnerMotor[j].pos = -1;
                }
            }

            pos = mMotorData[i].mDriverMotor[j].pos;
            if (pos >= 0 && mMotorData[i].mUnknown[j])
            {
                int value = mMotorData[i].mDriverMotor[j].pattern[pos];
                if (value == '*')
                {
                    patternValue = value;
                }
                else if (value == '-' && patternValue != '*')
                {
                    patternValue = value;
                }
            }

            s32 partnerPatternPos = mMotorData[i].mPartnerMotor[j].pos;
            if (partnerPatternPos >= 0)
            {
                int value = mMotorData[i].mPartnerMotor[j].pattern[partnerPatternPos];
                if (value == '*')
                {
                    patternValue = value;
                }
                else if (value == '-' && patternValue != '*')
                {
                    patternValue = value;
                }
            }

            if (pos >= 0)
            {
                mMotorData[i].mDriverMotor[j].pos += 1;
            }
            if (mMotorData[i].mPartnerMotor[j].pos >= 0)
            {
                mMotorData[i].mPartnerMotor[j].pos += 1;
            }

            mMotorData[i].mUnknown[j] = 0;
        }

        switch (patternValue)
        {
        case '-':
        {
            KartGamePad::PadPort padPort = mMotorData[i].mKartGamePad->getPadPort();
            if (0 <= padPort && padPort < KartGamePad::PORT_COUNT)
            {
                PADControlMotor(padPort, 0);
            }
            break;
        }
        case '*':
        {
            KartGamePad::PadPort padPort = mMotorData[i].mKartGamePad->getPadPort();
            if (0 <= padPort && padPort < KartGamePad::PORT_COUNT)
            {
                PADControlMotor(padPort, 1);
            }
            break;
        }
        case '/':
        {
            KartGamePad::PadPort padPort = mMotorData[i].mKartGamePad->getPadPort();
            if (0 <= padPort && padPort < KartGamePad::PORT_COUNT)
            {
                PADControlMotor(padPort, 2);
            }
            break;
        }
        default:
#line 363
            JUT_ASSERT(0);
        }
    }
}

void Motor::loopWave(KartGamePad *, int) {}
void Motor::loopWave(KartGamePad *, char *) {}
void Motor::checkLoop(int, const char *) {}

void Motor::triggerWave(KartGamePad *kartGamePad, int index)
{
    if (!kartGamePad)
        return;

    KartGamePad::PadPort padPort = kartGamePad->getPadPort();
    if (0 <= padPort && padPort < KartGamePad::PORT_COUNT)
    {
        mMotorData[padPort].mKartGamePad = kartGamePad;

        mMotorData[padPort].mPartnerMotor[mSomeCount].pattern = mPatternWave[index];
        mMotorData[padPort].mPartnerMotor[mSomeCount].pos = 0;

        if (++mSomeCount >= 4)
        {
            mSomeCount = 0;
        }
    }
}

void Motor::triggerWave(KartGamePad *, char *) {}

void Motor::powerWave(KartGamePad *kartGamePad, f32 strength, u8 phase1Duration, u8 phase2Duration)
{
    KartGamePad::PadPort padPort = kartGamePad->getPadPort();
    if (0 <= padPort && padPort < KartGamePad::PORT_COUNT)
    {
        static char patterns[KartGamePad::PORT_COUNT][512];

        f32 power = 0.0f;

        for (s32 duration = phase1Duration + phase2Duration, i = 0; duration > 0; --duration)
        {
            if (i < phase1Duration)
            {
                power += strength;

                if (power >= 1.0f)
                {
                    power = power - 1.0f;
                    patterns[padPort][i] = '*';
                }
                else
                {
                    patterns[padPort][i] = '-';
                }
            }
            else
            {
                power += strength * (1.0f - (i - phase1Duration) / (f32)phase2Duration);

                if (power >= 1.0f)
                {
                    power = power - 1.0f;
                    patterns[padPort][i] = '*';
                }
                else
                {
                    patterns[padPort][i] = '-';
                }
            }

            ++i;
        }

        char *pattern = patterns[padPort];
        pattern[phase1Duration + phase2Duration] = '\0';

        if (!kartGamePad)
            return;

        KartGamePad::PadPort padPort = kartGamePad->getPadPort();
        if (0 <= padPort && padPort < KartGamePad::PORT_COUNT)
        {
            mMotorData[padPort].mKartGamePad = kartGamePad;

            mMotorData[padPort].mPartnerMotor[mSomeCount].pattern = pattern;
            mMotorData[padPort].mPartnerMotor[mSomeCount].pos = 0;

            if (++mSomeCount >= 4)
            {
                mSomeCount = 0;
            }
        }
    }
}

void Motor::stop()
{
    PADControlMotor(0, 0);
    PADControlMotor(1, 0);
    PADControlMotor(2, 0);
    PADControlMotor(3, 0);

    for (int i = 0; i < RaceMgr::getManager()->getKartNumber(); ++i)
    {
        KartCtrl::getKartCtrl()->getKartBody(i)->getHandle()->StopMotor();
    }
}

#include "JSystem/JAudio/JASFakeMatch2.h"  // For static initializer
