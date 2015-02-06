
#ifndef _VISUAL_HEADER_H
#define _VISUAL_HEADER_H

#ifndef _PV_TYPES_ // In order to compile in MDF wrapper
#define _PV_TYPES_

typedef uint Bool;

#endif // #ifndef _PV_TYPES_


typedef struct tagVolInfo
{
    int32   shortVideoHeader;       /* shortVideoHeader mode */

    /* Error Resilience Flags */
    int32   errorResDisable;        /* VOL disable error resilence mode(Use Resynch markers) */
    int32   useReverseVLC;          /* VOL reversible VLCs */
    int32   dataPartitioning;       /* VOL data partitioning */

    /* Parameters used for scalability */
    int32   scalability;            /* VOL scalability (flag) */

    int32   nbitsTimeIncRes;        /* number of bits for time increment () */

    int32   profile_level_id;       /* profile and level */


} VolInfo;

#endif // #ifndef _VISUAL_HEADER_H

