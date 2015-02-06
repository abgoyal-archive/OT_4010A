
/************************************************************************/
/*  file name       : layer.h                                           */
/*  file contents   :                                                   */
/*  draw            :                                                   */
/*----------------------------------------------------------------------*/
/*  amendment       :                                                   */
/*      Copyright (C) 2002 PacketVideo                                  */
/************************************************************************/

#if !defined(LAYER_H)
#define LAYER_H

#include "packet_io.h"

class Layer
{
    public:
        enum
        {
            PACKET_INPUT = 0,
            PACKET_OUTPUT
        } TLayerNode;
        virtual ~Layer() {}
        OSCL_IMPORT_REF virtual PacketInput* GetPacketInput() = 0;
        OSCL_IMPORT_REF virtual PacketOutput* GetPacketOutput() = 0;
        OSCL_IMPORT_REF OsclAny SetBoundLayer(Layer* layer)
        {
            bound_layer = layer;
        }
        OSCL_IMPORT_REF virtual OsclAny Bind(Layer* layer);
        OSCL_IMPORT_REF virtual OsclAny UnBind();
        OSCL_IMPORT_REF virtual bool IsBound();
    protected:
        OSCL_IMPORT_REF Layer();
        Layer* GetBoundLayer();
        Layer* bound_layer;
};

class SimpleStackElement
{
    public:
        virtual ~SimpleStackElement() {}
        OSCL_IMPORT_REF virtual Layer* GetUpperLayer() = 0;
        OSCL_IMPORT_REF virtual Layer* GetLowerLayer() = 0;
        /*OSCL_IMPORT_REF virtual int Start(){ return 0;}
        OSCL_IMPORT_REF virtual int Stop(){ return 0;}*/

};

#endif /* LAYER_H */
