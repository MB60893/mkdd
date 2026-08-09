#include "Inagaki/GameSoundTable.h"
#include "JSystem/JAudio/Interface/JAISound.h"

#include "JSystem/JAudio/JASFakeMatch11.h"
#include "JSystem/JAudio/Interface/JAISe.h"
#include "JSystem/JAudio/Interface/JAISeq.h"
#include "JSystem/JAudio/Interface/JAIStream.h"
#include "JSystem/JAudio/System/JASDriver.h"
#include "JSystem/JUtility/JUTAssert.h"

namespace GameAudio {

u16 CustomSoundTable::getBgmSeqResourceID(JAISoundID soundID) const {
    #line 27
    JUT_ASSERT(isValid());

    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
    const u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    if(data != NULL) {
        switch(typeID) {
            case 0x20:
            return data->mSeqResourceID;
        }
    }
    return -1;
}

u32 CustomSoundTable::getSoundType(JAISoundID soundID) const {
    switch(soundID.mId.mBytes.mSectionId)
    {
        case 0: return 0;
        case 1: return 1;
        case 2: return 2;
        default: return -1;
    }
}

int CustomSoundTable::getCategory(JAISoundID soundID) const {
    if(soundID.mId.mBytes.mSectionId == 1) {
        return getPTrack(soundID.mId.mFullId) - 1;
    }

    return soundID.mId.mBytes.mGroupId;
}

u16 CustomSoundTable::getAudibleSw(JAISoundID soundID) const {
    #line 76
    JUT_ASSERT(isValid());

    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
    const u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    if(data != NULL) {
        switch(typeID) case 0x10:
            return data->mAudibleSw;
    }
    return -1;
}

u8 CustomSoundTable::getPTrack(JAISoundID soundID) const {
    #line 109
    JUT_ASSERT(isValid());

    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
    const u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    if(data != NULL) {
        switch(typeID) case 0x20:
            return data->mPTrack;
    }
    return -1;
}

u32 CustomSoundTable::getSwBit(JAISoundID soundID) const {
    #line 128
    JUT_ASSERT(isValid());

    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
    const u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    if(data != NULL) {
        switch(typeID){
            case 0x20:
                return data->mSwBit;
            case 0x10:
                return data->mSwBit;
        }
    }
    return -1;
}

u8 CustomSoundTable::getLevelFlag(JAISoundID soundID) const {
    #line 152
    JUT_ASSERT(isValid());

    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
    const u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    if(data != NULL) {
        switch(typeID){
            case 0x10:
                return data->mPriority != 0;
        }
    }
    return 0;
}

u32 CustomSoundTable::getPriority(JAISoundID soundID) const {
    #line 171
    JUT_ASSERT(isValid());

    JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);
    const u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    if(data != NULL) {
        switch(typeID){
            case 0x10:
                return 0xFF - data->_1;
            case 0x20:
                return 0xFF - data->mPriority;
            case 0x30:
                return 0xFF - data->mPriority;
        }
    }
    return 0;
}

void CustomSoundTable::getSeInfo(JAISoundID soundID, JAISe *se) const {
    getSoundInfo_(soundID, se);
}

void CustomSoundTable::getSeqInfo(JAISoundID soundID, JAISeq *seg) const {
    getSoundInfo_(soundID, seg);
}

void CustomSoundTable::getStreamInfo(JAISoundID soundID, JAIStream *stream) const {
    getSoundInfo_(soundID, stream);

    #line 209
    JUT_ASSERT(isValid());

    const u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    switch(typeID) {
    case 0x30:
        JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(soundID);

        #line 216
        JUT_ASSERT(data);

        s32 numChild = stream->getNumChild();

        const f32 leftPan = 0.f;
        const f32 rightPan = 1.f;

        for(s32 childIndex = 0; childIndex < numChild; childIndex++)
        {
            JAISoundChild* child = stream->getChild(childIndex);
            if(child == NULL)
            {
                continue;
            }

            if(childIndex == 0)
            {
                child->mMove.mPan = leftPan;
            }
            else if(childIndex == 1)
            {
                child->mMove.mPan = rightPan;
            }
        }
    }
}

void CustomSoundTable::getSoundInfo_(JAISoundID soundID, JAISound *sound) const {
    #line 257
    JUT_ASSERT(isValid());

    JAUSoundTable* soundTable = JASGlobalInstance<JAUSoundTable>::getInstance();
    JAUSoundTableItem* data = soundTable->getData(soundID);
    const u8 typeID = JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(soundID);

    if(data != NULL) {
        switch(typeID){
            case 0x10:
                sound->params_.mProperty._0 = data->_8 / 255.f;
            break;

            case 0x20:
                sound->params_.mProperty._0 = data->_8 / 255.f;
            break;

            case 0x30:
                sound->params_.mProperty._0 = data->_8 / 255.f;
                if(JASDriver::getOutputMode() == 0)
                {
                    sound->params_.mProperty._0 *= 0.75f;
                }
            break;
        }
    }
}

// UNUSED
void CustomSoundTable::getStreamFilePath(JAISoundID soundID){}

s32 CustomSoundTable::getStreamFileEntry(JAISoundID soundID) {
    const u32 ID = soundID.mId.mFullId;
    #line 297
    JUT_ASSERT(isValid());

    JASGlobalInstance<JAUSoundTable>::getInstance()->getTypeID(ID);

    return -1;
}

void CustomSoundTable::initAudibleSw() {
    JAISoundID id;
    id.mId.mFullId = 0;

    for (u8 group_id = 0;
         (s32)group_id < (s32)JASGlobalInstance<JAUSoundTable>::getInstance()->getNumGroups_inSection(0);
         group_id++)
    {
        for (u16 item = 0;
             (s32)item < (s32)JASGlobalInstance<JAUSoundTable>::getInstance()->getNumItems_inGroup(0, group_id);
             item++)
        {
            JAUSoundTableItem* data = JASGlobalInstance<JAUSoundTable>::getInstance()->getData(id);

            u16 acc = data->mAudibleSw & 0xC1;
            u32 swBit = getSwBit(id);

            acc += (u16)(((swBit >> 8) & 0x3) << 14);

            if ((swBit & 0x1) == 0)
            {
                acc += 0x800;
            }
            if ((swBit & 0x2) == 0)
            {
                acc += 0x400;
            }
            if ( swBit & 0x20)
            {
                acc += 0x100;
            }

            data->mAudibleSw = acc;

            id.mId.mAdvancedId.mShortId++;
        }
        id.mId.mAdvancedId.mShortId = 0;
        id.mId.mBytes.mGroupId++;
    }
}

}
