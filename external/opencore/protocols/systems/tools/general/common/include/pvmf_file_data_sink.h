
#ifndef PVMF_FILE_DATA_SINK_H_INCLUDED
#define PVMF_FILE_DATA_SINK_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef PVMF_NODE_INTERFACE_H_INCLUDED
#include "pvmf_node_interface.h"
#endif

#ifndef PVMF_PORT_BASE_IMPL_H_INCLUDED
#include "pvmf_port_base_impl.h"
#endif

#ifndef PVMF_SIMPLE_MEDIA_BUFFER_H_INCLUDED
#include "pvmf_simple_media_buffer.h"
#endif

#ifndef PVMF_MEDIA_DATA_H_INCLUDED
#include "pvmf_media_data.h"
#endif

#ifndef PVMI_CONFIG_AND_CAPABILITY_H_INCLUDED
#include "pvmi_config_and_capability_utils.h"
#endif

#ifndef PVMF_BUFFER_DATA_SINK_H_INCLUDED
#include "pvmf_buffer_data_sink.h"
#endif

class PVMFFileDataSink : public PVMFBufferDataSink
{
    public:
        OSCL_IMPORT_REF PVMFFileDataSink(int32 aPortTag);
        OSCL_IMPORT_REF virtual ~PVMFFileDataSink();

        // PVMFPortInterface virtuals
        PVMFStatus PutData(PVMFSharedMediaMsgPtr aMsg);

        bool OpenFile(char *aFileName)
        {
            iWriteFile = fopen(aFileName, "wb");
            if (iWriteFile)
            {
                return true;
            }
            return false;
        }

    private:
        FILE *iWriteFile;
};
#endif
