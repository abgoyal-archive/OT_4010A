

#ifndef OSCLCONFIG_ERROR_CHECK_H_INCLUDED
#define OSCLCONFIG_ERROR_CHECK_H_INCLUDED


#ifndef OSCL_HAS_EXCEPTIONS
#error "ERROR: OSCL_HAS_EXCEPTIONS has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_ERRNO_H
#error "ERROR: OSCL_HAS_ERRNO_H has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_SYMBIAN_ERRORTRAP
#error "ERROR: OSCL_HAS_SYMBIAN_ERRORTRAP has to be defined to either 1 or 0"
#endif

#ifndef OSCL_HAS_SETJMP_H
#error "ERROR: OSCL_HAS_SETJMP_H has to be defined to either 1 or 0"
#endif


#endif //OSCLCONFIG_ERROR_CHECK_H_INCLUDED


