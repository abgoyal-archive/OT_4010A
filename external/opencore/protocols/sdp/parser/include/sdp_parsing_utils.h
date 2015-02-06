

#ifndef SDP_PARSING_UTILS_H
#define SDP_PARSING_UTILS_H


bool get_next_line(const char *start_ptr, const char * end_ptr,
                   const char *& line_start,
                   const char *& line_end);

bool parseQoEMetrics(const char *start_ptr,
                     const char *end_ptr,
                     QoEMetricsType &qoeMetrics);
bool parseAssetInfo(const char *sptr, const char *line_end_ptr,
                    AssetInfoType &ainfo);

bool sdp_decodebase64(uint8* aInBuf, uint32 aInBufLen,
                      uint8* aOutBuf, uint32& aOutBufLen, uint32 aMaxOutBufLen);

void pvSDPParserGetAssetInfoLanguageCode(uint16 langcode, char* LangCode);

bool pvSDPParserParse3GPPAssetInfoLocation(AssetInfoType& aInfo,
        uint8* aBuf,
        uint32 aBufSize);

#endif


