
#ifndef PVMF_RECOGNIZER_PLUGIN_H_INCLUDED
#define PVMF_RECOGNIZER_PLUGIN_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef PVMF_RETURN_CODES_H_INCLUDED
#include "pvmf_return_codes.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef PVMF_FORMAT_TYPE_H_INCLUDED
#include "pvmf_format_type.h"
#endif

#ifndef PVMF_RECOGNIZER_TYPES_H_INCLUDED
#include "pvmf_recognizer_types.h"
#endif

#ifndef PVMI_DATASTREAMUSER_INTERFACE_H_INCLUDED
#include "pvmi_datastreamuser_interface.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif
class PVMFRecognizerPluginInterface
{
    public:
        /**
         * Virtual destructor for the plug-in. All plug-ins should perform any clean up here
         **/
        virtual ~PVMFRecognizerPluginInterface()
        {
        };

        /**
         * This methods returns a list of format(s) that this plug-in can recognize. Each supported format
         * is represented by a MIME string.
         *
         * @param aSupportedFormatsList
         *        Reference to a list of MIME strings which will be filled in with list of formats that the plug-in can recognize.
         *
         * @exception This method can leave with one of the following error codes
         *         OsclErrNoMemory if memory cannot be allocated for the format list
         *
         * @returns A PVMF status code to report result of method
         **/
        virtual PVMFStatus SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList) = 0;

        /**
         * This method determines the the specified content is or is not one of the formats recognized by this plug-in
         *
         * @param aSourceDataStreamFactory
         *        A reference to a PVMFDataStreamFactory representing the content to recognize
         * @param aFormatHint
         *        An optional input parameter expressed as a list of MIME string which provides a priori hint for the format
         *        of the content specified by aSourceDataStreamFactory.
         * @param aRecognizerResult
         *        An output parameter which is a reference to a vector of PVMFRecognizerResult that will contain the recognition
         *        result if the Recognize() method succeeds.
         *
         * @exception This method can leave with one of the following error codes
         *
         * @returns A PVMF status code to report result of method
         **/
        virtual PVMFStatus Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory,
                                     PVMFRecognizerMIMEStringList* aFormatHint,
                                     Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult) = 0;

        /**
         * This method returns the mininum required bytes in datastream for this plug-in
         * to be able to recognize its supported formats.
         *
         * @param aBytes[out]
         *        A reference to a minimum required bytes
         *
         * @returns A PVMF status code to report result of method.
         * PVMFSuccess in case of success and PVMFFailure otherwise.
         **/
        virtual PVMFStatus GetRequiredMinBytesForRecognition(uint32& aBytes) = 0;
};


class PVMFRecognizerPluginFactory: public HeapBase
{
    public:
        /**
         * Virtual destructor for the plug-in factory. All plug-in factory should perform any clean up here
         **/
        virtual ~PVMFRecognizerPluginFactory()
        {
        };

        /**
         * This method instantiates and returns the recognizer plug-in that the factory is associated with.
         *
         * @exception This method can leave with one of the following error codes
         *         OsclErrNoMemory if memory cannot be allocated for the recognizer plug-in
         * @returns A pointer to the recognizer plug-in instance if creation is successful.
         **/
        virtual PVMFRecognizerPluginInterface* CreateRecognizerPlugin() = 0;

        /**
         * This method destroys the specified recognizer plug-in pointer as the particular recognizer plug-in
         * the factory is associated with.
         *
         * @param aPlugIn
         *        A pointer to the recognizer plug-in that should be destroyed.
         *
         * @exception This method can leave with one of the following error codes
         *
         * @returns None
         **/
        virtual void DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugIn) = 0;
};


template<class T>
class PVMFRecognizerPluginFactoryBasic : public PVMFRecognizerPluginFactory
{
    public:
        virtual ~PVMFRecognizerPluginFactoryBasic()
        {
        };

        PVMFRecognizerPluginInterface* CreateRecognizerPlugin()
        {
            T* plugin = OSCL_NEW(T, ());
            return plugin;
        };

        void DestroyRecognizerPlugin(PVMFRecognizerPluginInterface* aPlugIn)
        {
            T* plugin = (T*)aPlugIn;
            OSCL_DELETE(plugin);
        };
};

#endif // PVMF_RECOGNIZER_PLUGIN_H_INCLUDED




