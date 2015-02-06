
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//   O S C L C O N F I G _ T I M E   ( T I M E - D E F I N I T I O N S )

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCLCONFIG_TIME_H_INCLUDED
#define OSCLCONFIG_TIME_H_INCLUDED


#ifndef OSCLCONFIG_H_INCLUDED
#include "osclconfig.h"
#endif

// system header files
#include <time.h> // timeval
#include <sys/time.h> // timercmp
#include <unistd.h>



#define OSCL_HAS_UNIX_TIME_FUNCS        1


typedef struct timeval OsclBasicTimeStruct;
typedef tm      OsclBasicDateTimeStruct;

#include "osclconfig_time_check.h"

#endif
