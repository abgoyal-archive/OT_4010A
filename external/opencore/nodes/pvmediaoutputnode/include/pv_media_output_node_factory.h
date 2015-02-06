
#ifndef PV_MEDIA_OUTPUT_NODE_FACTORY_H_INCLUDED
#define PV_MEDIA_OUTPUT_NODE_FACTORY_H_INCLUDED

// Forward declaration
class PVMFNodeInterface;
class PvmiMIOControl;
class PVMFNodeCmdStatusObserver;
class PVMFNodeInfoEventObserver;
class PVMFNodeErrorEventObserver;

//6418b1e0-f020-11d9-8cd6-0800200c9a66
#define PVMFMediaOutputNodeUUID PVUuid(0x6418b1e0,0xf020,0x11d9,0x8c,0xd6,0x08,0x00,0x20,0x0c,0x9a,0x66)


class PVMediaOutputNodeFactory
{
    public:
        /**
         * Creates an instance of a PVMediaOutputNode. If the creation fails, this function will leave.
         *
         * @param aMIOControl            Media IO object to be controlled by this node
         * @returns A pointer to an author or leaves if instantiation fails
         */
        OSCL_IMPORT_REF static PVMFNodeInterface* CreateMediaOutputNode(
            PvmiMIOControl* aMIOControl);

        /**
         * This function allows the application to delete an instance of a PVMediaOutputNode
         * and reclaim all allocated resources.  An instance can be deleted only in
         * the idle state. An attempt to delete in any other state will fail and return false.
         *
         * @param aNode The PVMediaOutputNode to be deleted.
         */
        OSCL_IMPORT_REF static void DeleteMediaOutputNode(PVMFNodeInterface* aNode);

        /**
         * This function allows the application to retrieve the
         * media I/O control instance contained within the node.
         *
         * @param aNode The PVMediaOutputNode
         * @returns Pointer to the media I/O component.
         */
        OSCL_IMPORT_REF static PvmiMIOControl* GetMediaIOControl(PVMFNodeInterface* aNode);
};



#endif // PV_MEDIA_OUTPUT_NODE_FACTORY_H_INCLUDED
