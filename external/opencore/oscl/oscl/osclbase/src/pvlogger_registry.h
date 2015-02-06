
#ifndef PVLOGGER_REGISTRY_H_INCLUDED
#define PVLOGGER_REGISTRY_H_INCLUDED

#ifndef PVLOGGER_H_INCLUDED
#include "pvlogger.h"
#endif

#ifndef OSCL_TAGTREE_H_INCLUDED
#include "oscl_tagtree.h"
#endif


class PVLoggerRegistry
{
    public:

        typedef PVLogger::log_level_type log_level_type;
        typedef PVLogger::alloc_type alloc_type;

        /**
         * Get the logger registry.  There is only one logger
         * registry instance per thread.
         */
        OSCL_IMPORT_REF static PVLoggerRegistry* GetPVLoggerRegistry();

        /**
         * PVLoggerRegistry Constructor
         *
         */
        OSCL_IMPORT_REF PVLoggerRegistry();

        /**
         * PVLoggerRegistry Destructor
         *
         */
        OSCL_IMPORT_REF virtual ~PVLoggerRegistry();

        /**
         * PVLoggerRegistry method to get access to a logging control point, associated with
         * a tag. In case the logger for this tag does not exist, it is created afresh, else
         * pointer to the existing one is returned.
         *
         * @param inputTag   logger tag, viz. "x.y.z"
         * @param level      log level associated with the logging control point
         *
         * @param oAppenderInheritance
         *
         * @return PVLogger<Alloc, TheLock>* Pointer to the logging control point
         */
        OSCL_IMPORT_REF PVLogger *GetPVLoggerObject(const char* tagIn);

        /**
         * This method creates a log control point, with specified tag, and level
         *
         * @param inputTag   logger tag, viz. "x.y.z"
         * @param level      log level associated with the logging control point
         *
         * @param oAppenderInheritance
         *
         * @return PVLogger<alloc_type, TheLock>* Pointer to the logging control point
         */
        OSCL_IMPORT_REF PVLogger *CreatePVLogger(const char* tagIn, log_level_type level, bool oAppenderInheritance);

        /**
         * This method propagates the log level to all the descendents of the node,
         * with a specified tag.
         *
         * @param tagIn      logger tag, viz. "x.y.z"
         * @param level      log level associated with the logging control point
         *
         * @return           true on success, else false.
         */
        OSCL_IMPORT_REF bool SetNodeLogLevelExplicit(char* tagIn, log_level_type level);

        /**
         * This method recursively propagates the log level to all the descendents,
         * of a node.
         *
         * @param node       Node ptr, associated with a logger, from the tag tree.
         * @param level      log level associated with the logging control point
         *
         * @return           NONE
         */
        OSCL_IMPORT_REF void SetNodeLogLevelExplicit(Oscl_TagTree<PVLogger*, alloc_type>::node_type* node, log_level_type level);

    private:
#if (PVLOGGER_ENABLE)
        Oscl_TAlloc<PVLogger, alloc_type> _pvloggerAlloc;
        Oscl_TagTree<PVLogger*, alloc_type> _loggerTree;
#endif
};

#endif // PVLOGGER_REGISTRY_H_INCLUDED

