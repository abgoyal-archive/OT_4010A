
#ifndef mode_h
#define mode_h "$Id $"



#ifdef __cplusplus
extern "C"
{
#endif

    /*
    ********************************************************************************
    *                         DEFINITION OF DATA TYPES
    ********************************************************************************
    */
    enum Mode { MR475 = 0,
        MR515,
        MR59,
        MR67,
        MR74,
        MR795,
        MR102,
        MR122,

        MRDTX,

        N_MODES     /* number of (SPC) modes */

    };

#ifdef __cplusplus
}
#endif

#endif
