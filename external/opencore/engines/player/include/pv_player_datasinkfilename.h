
#ifndef PV_PLAYER_DATASINKFILENAME_H_INCLUDED
#define PV_PLAYER_DATASINKFILENAME_H_INCLUDED

#ifndef PV_PLAYER_DATASINK_H_INCLUDED
#include "pv_player_datasink.h"
#endif

class PVPlayerDataSinkFilename : public PVPlayerDataSink
{
    public:
        PVPlayerDataSinkFilename() : iFormatType(PVMF_MIME_FORMAT_UNKNOWN), iFilename(NULL) {};
        ~PVPlayerDataSinkFilename() {};

        PVPDataSinkType GetDataSinkType()
        {
            return PVP_DATASINKTYPE_FILENAME;
        }

        PVMFFormatType GetDataSinkFormatType()
        {
            return iFormatType;
        }

        OSCL_wString& GetDataSinkFilename()
        {
            return iFilename;
        }

        PVMFNodeInterface* GetDataSinkNodeInterface()
        {
            return NULL;
        }

        void SetDataSinkFormatType(PVMFFormatType aFormatType)
        {
            iFormatType = aFormatType;
        }

        void SetDataSinkFilename(const OSCL_wString& aFilename)
        {
            iFilename = aFilename;
        }

    private:
        PVMFFormatType iFormatType;
        OSCL_wHeapString<OsclMemAllocator> iFilename;
};

#endif // PV_PLAYER_DATASINKFILENAME_H_INCLUDED

