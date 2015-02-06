
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//               O S C L _ P R O F I L E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_FILE_CFG_H_INCLUDED
#define OSCL_FILE_CFG_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#define PLAYER_CFG_FILE_NAME "player.cfg"

class Oscl_File_Cfg
{
    public:
        enum CFG_TYPE
        {
            CFG_PLAYER  = 1,
            CFG_AUTHOR  = 2,
            CFG_2WAY    = 3
        };

#define CFG_MAX CFG_2WAY
        static int LoadCfg(CFG_TYPE aType, char* aCfgItem);
};

#endif // OSCL_FILE_CFG_H_INCLUDED

/*! @} */
