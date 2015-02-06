
/*                                                                               */
/*********************************************************************************/


#ifndef RTCP_ENCODER_H
#define RTCP_ENCODER_H

#ifndef OSCL_TYPES_H_INCLUDED
#include "oscl_types.h"
#endif
#ifndef OSCL_BIN_STREAM_H_INCLUDED
#include "oscl_bin_stream.h"
#endif
#ifndef RTCP_H
#include "rtcp.h"
#endif

const int32 RTCP_ENCODER_MAX_CNAME_SIZE = 255;


class RTCP_Encoder : public RTCP_Base
{
    public:

        typedef enum
        {
            RTCP_SUCCESS,
            FAIL,
            OUTPUT_TRUNCATED,
            INVALID_PAD_LENGTH
        } Error_t;

        OSCL_IMPORT_REF    RTCP_Encoder(const uint8 * cname = NULL,
                                        int32 cname_length = 0,
                                        const uint32 ssrc = 0,
                                        const uint8 version = DEFAULT_RTPRTCP_VERSION);
        ~RTCP_Encoder() {};

        void SetSSRC(uint32 new_ssrc)
        {
            SSRC = new_ssrc;
        };

        void setCName(const uint8 * cname,
                      int32 cname_length);

        /*
        ** Description:
        **  This is a convenience function to put together the
        **  typical compound RTCP containing an RR, an SDES, and
        **  optionally an APP packet.  Set the APP packet to NULL
        **  to skip it.
        **
        **  The output_buffer is an input/output parameter.  It should
        **  be initialized to the memory location and size of the buffer
        **  where the data should be written.  When the function returns
        **  it will contains the ptr to the buffer and the actual size
        **  written unless there was not enough room (i.e., return status
        **  is OUTPUT_TRUNCATED).  In that case, the ptr will be NULL, and
        **  the size will be size required to write everything.
        */
        OSCL_IMPORT_REF    Error_t EncodeCompoundRR(const RTCP_RR& rr_packet,
                OsclMemoryFragment& output_buffer,
                const RTCP_APP* app_packet,
                uint8 pad_length = 0);


        /*
        ** Description:
        **  This is a convenience function to put together the
        **  typical compound RTCP containing an SR and an SDES,
        **
        **  Behavior is similar to EncodeCompoundRR above.
        */
        Error_t EncodeCompoundSR(const RTCP_SR& sr_packet,
                                 OsclMemoryFragment& output_buffer,
                                 uint8 pad_length = 0);


        Error_t EncodeRR(const RTCP_RR& rr_packet,
                         OsclMemoryFragment& output_buffer,
                         uint8 pad_length = 0);

        Error_t EncodeSR(const RTCP_SR& sr_packet,
                         OsclMemoryFragment& output_buffer,
                         uint8 pad_length = 0);



        Error_t EncodeBYE(const RTCP_BYE& bye_packet,
                          OsclMemoryFragment& output_buffer,
                          uint8 pad_length = 0);

        Error_t EncodeAPP(const RTCP_APP& app_packet,
                          OsclMemoryFragment& output_buffer,
                          uint8 pad_length = 0);

        Error_t EncodeSDES(const RTCP_SDES& sdes_packet,
                           OsclMemoryFragment& output_buffer,
                           uint8 pad_length = 0);


        int32 GetEncodedSize(const RTCP_RR& rr_packet);

        int32 GetEncodedSize(const RTCP_SR& sr_packet);

        int32 GetEncodedSize(const RTCP_SDES& sdes_packet);

        int32 GetEncodedSize(const RTCP_APP& app_packet);








    private:

        uint8 cName[RTCP_ENCODER_MAX_CNAME_SIZE];
        uint8 cName_length;
        uint32 SSRC;

        Error_t EncodeReportBlock(OsclBinOStreamBigEndian & outStream,/* Input
                                                                 * stream
                                                                 * reference
                                                                 */
                                  const RTCP_ReportBlock* report
                                 );


        Error_t output_rtcp_header(uint8 packet_type,
                                   uint8 count_field,
                                   uint16 size,
                                   OsclBinOStreamBigEndian& outStream,
                                   bool pad_bit);


        Error_t EncodeSDESItem(OsclBinOStreamBigEndian& outStream,
                               const SDES_item* item_ptr);


        Error_t EncodeSDESChunk(OsclBinOStreamBigEndian& outStream,
                                const SDES_chunk* chunk_ptr);


};

#endif
