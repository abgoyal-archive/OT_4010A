
#ifndef PVMF_OMX_BASEDEC_NODE_EXTENSION_INTERFACE_H_INCLUDED
#define PVMF_OMX_BASEDEC_NODE_EXTENSION_INTERFACE_H_INCLUDED

struct PVMFOMXBaseDecNodeConfig
{
    bool iPostProcessingEnable;
    int32 iPostProcessingMode;
    bool iDropFrame;
    PVMFFormatType iMimeType;
};

//Mimetype and Uuid for the custom interface
#define PVMF_OMX_BASE_DEC_NODE_CUSTOM1_UUID 1,2,3,0xde,0xad,0xae,0xcf,0x20,0x11,0x73,0x33

//Mimetypes for the custom interface
#define PVMF_OMX_BASE_DEC_NODE_CUSTOM1_MIMETYPE "pvxxx/OMXBaseDecNode/Custom1"

class PVMFOMXBaseDecNodeExtensionInterface : public PVInterface
{
    public:
        //**********begin PVInterface
        virtual void addRef() = 0;
        virtual void removeRef() = 0;
        virtual bool queryInterface(const PVUuid& uuid, PVInterface*& iface) = 0;
        //**********end PVInterface

        virtual PVMFStatus SetDecoderNodeConfiguration(PVMFOMXBaseDecNodeConfig& aConfig) = 0;

};

#endif //PVMF_OMX_BASEDEC_NODE_EXTENSION_INTERFACE_H_INCLUDED


