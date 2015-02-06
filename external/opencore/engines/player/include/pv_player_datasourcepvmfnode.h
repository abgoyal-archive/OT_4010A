
#ifndef PV_PLAYER_DATASOURCEPVMFNODE_H_INCLUDED
#define PV_PLAYER_DATASOURCEPVMFNODE_H_INCLUDED

#ifndef PV_PLAYER_DATASOURCE_H_INCLUDED
#include "pv_player_datasource.h"
#endif

class PVPlayerDataSourcePVMFNode : public PVPlayerDataSource
{
    public:
        PVPlayerDataSourcePVMFNode() : iFormatType(PVMF_FORMAT_UNKNOWN), iNode(NULL), iEmptyString(NULL) {};
        ~PVPlayerDataSourcePVMFNode() {};

        PVPDataSourceType GetDataSourceType()
        {
            return PVP_DATASRCTYPE_SOURCENODE;
        }

        PVMFFormatType GetDataSourceFormatType()
        {
            return iFormatType;
        }

        OSCL_wString& GetDataSourceURL()
        {
            return iEmptyString;
        }

        OsclAny* GetDataSourceContextData()
        {
            return NULL;
        }

        PVMFNodeInterface* GetDataSourceNodeInterface()
        {
            return iNode;
        }

        void SetDataSourceFormatType(PVMFFormatType aFormatType)
        {
            iFormatType = aFormatType;
        }

        void SetDataSourceNode(PVMFNodeInterface* aNode)
        {
            iNode = aNode;
        }

        bool SetAlternateSourceFormatType(PVMFFormatType aFormatType)
        {
            return false;
        }

        uint32 GetNumAlternateSourceFormatTypes()
        {
            return 0;
        }

        bool GetAlternateSourceFormatType(PVMFFormatType& aFormatType,
                                          uint32 aIndex)
        {
            return false;
        }

    private:
        PVMFFormatType iFormatType;
        PVMFNodeInterface* iNode;
        OSCL_wStackString<1> iEmptyString;
};

#endif // PV_PLAYER_DATASOURCEPVMFNODE_H_INCLUDED

