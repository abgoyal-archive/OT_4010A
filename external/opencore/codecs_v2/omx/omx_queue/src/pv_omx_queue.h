



#ifndef PV_OMX_QUEUE_H_INCLUDED
#define PV_OMX_QUEUE_H_INCLUDED

#ifndef OSCL_MEM_H_INCLUDED
#include "oscl_mem.h"
#endif

#ifndef OMX_Types_h
#include "OMX_Types.h"
#endif

#ifndef OMX_Core_h
#include "OMX_Core.h"
#endif

/* Queue Depth i.e maximum number of elements */
#define MAX_QUEUE_ELEMENTS 12


/* Queue Element Type */
struct QueueElement
{
    QueueElement* pQueueNext;
    void* pData;
};

typedef struct QueueType
{
    QueueElement* pFirst;   /** Queue Front */
    QueueElement* pLast;    /** Queue Rear (last filled element of queue) */
    OMX_S32 NumElem;        /** Number of elements currently in the queue */
    OMX_S32 NumElemAdded;   /** Number of elements added extra at run time*/
} QueueType;



//QUEUE OPERATIONS

/* Queue initialization routine */
OSCL_IMPORT_REF OMX_ERRORTYPE QueueInit(QueueType* aQueue);

/* Queue deinitialization routine */
OSCL_IMPORT_REF void QueueDeinit(QueueType* aQueue);

/* Function to queue an element in the given queue*/
OSCL_IMPORT_REF OMX_ERRORTYPE Queue(QueueType* aQueue, void* aData);

OSCL_IMPORT_REF void* DeQueue(QueueType* aQueue);

/* Function to get number of elements currently in the queue */
OSCL_IMPORT_REF OMX_S32 GetQueueNumElem(QueueType* aQueue);

/* Add new element in the queue if required */
OSCL_IMPORT_REF OMX_BOOL AddQueueElem(QueueType* aQueue);

#endif      //#ifndef PV_OMX_QUEUE_H_INCLUDED
