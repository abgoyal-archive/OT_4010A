
#ifndef PV_PLAYER_DATASINKPVMFNODE_H_INCLUDED
#define PV_PLAYER_DATASINKPVMFNODE_H_INCLUDED

#ifndef PV_PLAYER_DATASINK_H_INCLUDED
#include "pv_player_datasink.h"
#endif

class PVPlayerDataSinkPVMFNode : public PVPlayerDataSink
{
    public:
        PVPlayerDataSinkPVMFNode() : iFormatType(PVMF_MIME_FORMAT_UNKNOWN), iNode(NULL), iEmptyString(NULL) {};
        ~PVPlayerDataSinkPVMFNode() {};

        PVPDataSinkType GetDataSinkType()
        {
            return PVP_DATASINKTYPE_SINKNODE;
        }

        PVMFFormatType GetDataSinkFormatType()
        {
            return iFormatType;
        }

        OSCL_wString& GetDataSinkFilename()
        {
            return iEmptyString;
        }

        PVMFNodeInterface* GetDataSinkNodeInterface()
        {
            return iNode;
        }

        void SetDataSinkFormatType(PVMFFormatType aFormatType)
        {
            iFormatType = aFormatType;
        }

        void SetDataSinkNode(PVMFNodeInterface* aNode)
        {
            iNode = aNode;
        }

    private:
        PVMFFormatType iFormatType;
        PVMFNodeInterface* iNode;
        OSCL_wStackString<1> iEmptyString;
};

#endif // PV_PLAYER_DATASINKPVMFNODE_H_INCLUDED

