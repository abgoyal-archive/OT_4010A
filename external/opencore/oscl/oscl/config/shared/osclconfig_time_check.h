

//osclconfig: this build configuration file is for win32
#ifndef OSCLCONFIG_TIME_CHECK_H_INCLUDED
#define OSCLCONFIG_TIME_CHECK_H_INCLUDED


#ifndef OSCL_HAS_UNIX_TIME_FUNCS
#error "ERROR: OSCL_HAS_UNIX_TIME_FUNCS has to be defined to either 1 or 0"
#endif

typedef OsclBasicTimeStruct __Validate__BasicTimeStruct__;

typedef OsclBasicDateTimeStruct __Validate__BasicTimeDateStruct__;

#endif //OSCLCONFIG_TIME_CHECK_H_INCLUDED


