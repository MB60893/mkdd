#ifndef DRIVERMGR_H
#define DRIVERMGR_H

#include "JSystem/J3D/J3DModel.h"
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "Osako/ResMgr.h"
#include "macros.h"
#include "types.h"

class DriverModel;

class DriverLoader {
public:

    enum AnmEnum {};
    enum AnmSoundEnum {};

    DriverLoader(int);
    static int convAnmIDToKeepID(DriverLoader::AnmEnum);
    void *getAnmFile(int);
    static int convSndIDToKeepID(DriverLoader::AnmSoundEnum);
    void *getSoundDataFile(int);
    static J3DModelData *createModelData(int dataID, int level);

    // Inline/Unused
    static void *getBmdFile(int, int);

    inline static const char *getDriverName(int no) {
#line 206
        JUT_MINMAX_ASSERT(0, no, 20);
        return mscpaDriverName[no];
    }

    inline static const char *getDriverFolderName2(int no) {
#line 212
        JUT_MINMAX_ASSERT(0, no, 20);
        return mscpaDriverFolderName2[no];
    }

    static const u8 mscAnm2SoundList[82];
    static const u8 msAnmMRAMList[11];
    static const char mscaaMRAMAnmFileName[11][32];
    static const char mscaaMRAMAnmSoundFileName[1][32];
    static const char *sAnmFileNameTable[82];
    static const char *sAnmSoundFileNameTable[12];
    static const char *mscpaDriverName[20];
    static const char *mscpaDriverFolderName2[20];
    static const char *mscaaJntName[27];
    static const char *sModelFileNameTable[2];
    static const char *sDriverFolderName3;
    static const u8 msAnmSoundMRAMList[1];
    static const u16 mscaBoneJnt[4];

private:
    int mDataID;
    J3DModelData *mMdlData[2];
    u16 _c[27];
    f32 _44[4];
}; // class DriverLoader

class DriverManager : JKRDisposer {
public:
    DriverManager();
    ~DriverManager();
    static DriverManager *ptr();
    const char *getFolderName_Sound(int, int);
    void SetDriverModelPtr(DriverModel *);

    // Inline/Unused
    bool IsAllARAMDMAFinished();
    void CheckARAMDMA(f64, int);

    static DriverManager *mspDriverManager;
private:
    f32 _18;
    f32 _1c;
    f32 _20;
    u8 _24;
    f32 _28;
    u8 _2c;
    J3DModelData *mMdlData;
    u8 _34;
    u8 _35;
    u8 _36;
    u8 _37;
    u8 _38;
    u8 _39;
    u8 _3a;
    u8 _3b;
    u8 _3c;
    u8 _3d;
    u8 _3e;
    u8 _3f;
    u8 _40;
    u8 _41;
    u8 _42;
    u8 _43;
    u8 _44;
    u8 _45;
    u8 _46;
    u8 _47;
    u8 _48;
    u8 _49;
    u8 _4a;
    u8 _4b;
    u8 _4c;
    u8 _4d;
    u8 _4e;
    u8 _4f;
    u8 _50;
    u8 _51;
    u8 _52;
    u8 _53;
    u8 _54;
    u8 _55;
    u8 _56;
    u8 _57;
    u8 _58;
    u8 _59;
    u8 _5a;
    u8 _5b;
    u8 _5c;
    u8 _5d;
    u8 _5e;
    u8 _5f;
    u8 _60[20][82];
    u8 _6c8[20][12];
    u8 _7b8[20];
    u8 _7cc[20];
    u8 _7e0[20];
    u8 mMdlCnt;                      // 7f4
    DriverModel *mMdls[16];          // 7f8, unknown count as of now
    u16 _838;
    u16 _83a;
    u16 _83c;
    f64 _840;
    int _848;
    PLACEHOLDER_BYTES(0x84c, 0x858);
    // Size: 0x858 
}; // class DriverManager


#endif // DRIVERMGR_H
