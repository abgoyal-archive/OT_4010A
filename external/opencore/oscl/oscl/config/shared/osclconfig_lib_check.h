

#ifndef OSCLCONFIG_LIB_CHECK_H_INCLUDED
#define OSCLCONFIG_LIB_CHECK_H_INCLUDED




#if !defined(OSCL_HAS_RUNTIME_LIB_LOADING_SUPPORT )
#error "ERROR: OSCL_HAS_RUNTIME_LIB_LOADING_SUPPORT must be defined to 0 or 1"
#endif

#if(OSCL_HAS_RUNTIME_LIB_LOADING_SUPPORT)
#if !defined(OSCL_LIB_READ_DEBUG_LIBS)
#error "ERROR: OSCL_LIB_READ_DEBUG_LIBS must be defined to 0 or 1"
#endif

#if !defined(PV_DYNAMIC_LOADING_CONFIG_FILE_PATH)
#error "ERROR: PV_DYNAMIC_LOADING_CONFIG_FILE_PATH must be set to a path where the config files are expected to be present"
#endif

#if !defined(PV_RUNTIME_LIB_FILENAME_EXTENSION)
#error "ERROR: PV_RUNTIME_LIB_FILENAME_EXTENSION must be specified for use as the dynamic library file extension"
#endif

#endif // OSCL_HAS_RUNTIME_LIB_LOADING_SUPPORT


/*! @} */

#endif // OSCLCONFIG_LIB_CHECK_H_INCLUDED


