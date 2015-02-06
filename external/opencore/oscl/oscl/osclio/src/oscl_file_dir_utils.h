
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ F I L E  _ Dir _ utils

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_FILE_DIR_UTILS_H_INCLUDED
#define OSCL_FILE_DIR_UTILS_H_INCLUDED

#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif



typedef struct oscl_fsstat
{
    uint64 freebytes;
    uint64 totalbytes;
} OSCL_FSSTAT;

typedef enum
{
    OSCL_FILEMGMT_PERMS_READ = 0x1,
    OSCL_FILEMGMT_PERMS_WRITE = 0x2,
    OSCL_FILEMGMT_PERMS_EXECUTE = 0x4,
} OSCL_FILEMGMT_PERMS;

typedef enum
{
    OSCL_FILEMGMT_MODE_DIR = 0x1
} OSCL_FILEMGMT_MODES;

typedef struct oscl_stat_buf
{
    uint32 mode;
    uint32 perms;
} OSCL_STAT_BUF;

typedef enum
{
    OSCL_FILEMGMT_E_OK = 0,
    OSCL_FILEMGMT_E_PATH_TOO_LONG,
    OSCL_FILEMGMT_E_PATH_NOT_FOUND,
    OSCL_FILEMGMT_E_ALREADY_EXISTS,
    OSCL_FILEMGMT_E_NOT_EMPTY,
    OSCL_FILEMGMT_E_PERMISSION_DENIED,
    OSCL_FILEMGMT_E_NO_MATCH,
    OSCL_FILEMGMT_E_UNKNOWN,
    OSCL_FILEMGMT_E_SYS_SPECIFIC,
    OSCL_FILEMGMT_E_NOT_IMPLEMENTED
} OSCL_FILEMGMT_ERR_TYPE;

OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_getcwd(oscl_wchar *path, uint32 size);


OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_getcwd(char *path, uint32 size);



OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_stat(const oscl_wchar *path, OSCL_STAT_BUF *statbuf);


OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_stat(const char *path, OSCL_STAT_BUF *statbuf);


OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_mkdir(const oscl_wchar *path);

OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_mkdir(const char *path);

OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_rmdir(const oscl_wchar *path);

OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_rmdir(const char *path);

OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_chdir(const oscl_wchar *path);

OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_chdir(const char *path);


OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_rename(const oscl_wchar *oldpath, const oscl_wchar *newpath);


OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_rename(const char *oldpath, const char *newpath);

OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_statfs(OSCL_FSSTAT *stats, const char *path);
OSCL_IMPORT_REF OSCL_FILEMGMT_ERR_TYPE oscl_statfs(OSCL_FSSTAT *stats, const oscl_wchar *path);


#endif // OSCL_FILE_DIR_UTILS_H_INCLUDED

/*! @} */

