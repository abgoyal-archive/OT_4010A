 
 /*****************************************************************************
 *
 * Filename:
 * ---------
 *   pvmf_rmffparser_node_metadata.cpp
 *
 * Project:
 * --------
 *   YUSU
 *
 * Description:
 * ------------
 *   Implement RMFF parser node
 *
 * Author:
 * -------
 *   Morris Yang (mtk03147)
 *
 *============================================================================
 *             HISTORY
 * Below this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *------------------------------------------------------------------------------
 * $Revision$
 * $Modtime $
 * $Log     $  
 *
 * 01 13 2011 donglei.ji
 * [ALPS00139474] [Need Patch] [Volunteer Patch] Android2.3 opencore migration
 * Android2.3 opencore migration.
 *
 * 08 13 2010 techien.chen
 * [ALPS00003391] [Need Patch] [Volunteer Patch] ALPS.W33 Volunteer patch for Android 2.2 migration, opencore part.
 * opencore 2.2 migration. Disable video HW related in this version.
 *
 * 08 13 2010 techien.chen
 * [ALPS00003391] [Need Patch] [Volunteer Patch] ALPS.W33 Volunteer patch for Android 2.2 migration, opencore part.
 * opencore 2.2 migration. Disable video HW related in this version.
 *------------------------------------------------------------------------------
 * Upper this line, this part is controlled by PVCS VM. DO NOT MODIFY!!
 *============================================================================
 ****************************************************************************/
#include "pvmf_rmffparser_node.h"

#include "media_clock_converter.h"

#include "pv_mime_string_utils.h"

#include "oscl_snprintf.h"

#include "pvmf_duration_infomessage.h"

#include "pvmi_kvp_util.h"

#include "oscl_exclusive_ptr.h"

#include <cutils/log.h>

// Constant character strings for metadata keys
static const char PVRM_ALL_METADATA_KEY[] = "all";
static const char PVRMMETADATA_CLIP_TYPE_KEY[] = "clip-type";
static const char PVRMMETADATA_ALBUM_KEY[] = "album";
static const char PVRMMETADATA_COMMENT_KEY[] = "comment";


static const char PVRMETADATA_LOCATION_KEY[] = "location;format=3GPP_LOCATION";
static const char PVRMETADATA_YEAR_KEY[] = "year";
static const char PVRMMETADATA_AUTHOR_KEY[] = "author";
static const char PVRMMETADATA_ARTIST_KEY[] = "artist";
static const char PVRMMETADATA_GENRE_KEY[] = "genre";
static const char PVRMMETADATA_KEYWORD_KEY[] = "keyword";
static const char PVRMMETADATA_CLASSIFICATION_KEY[] = "classification";
static const char PVRMMETADATA_TITLE_KEY[] = "title";
static const char PVRMMETADATA_DESCRIPTION_KEY[] = "description";
static const char PVRMMETADATA_RATING_KEY[] = "rating";
static const char PVRMMETADATA_COPYRIGHT_KEY[] = "copyright";
static const char PVRMMETADATA_VERSION_KEY[] = "version";
static const char PVRMMETADATA_DATE_KEY[] = "date";
static const char PVRMMETADATA_DURATION_KEY[] = "duration";
static const char PVRMMETADATA_NUMTRACKS_KEY[] = "num-tracks";
static const char PVRMMETADATA_IS_MOOF_KEY[] = "movie-fragments-present";

static const char PVRMMETADATA_TOOL_KEY[] = "tool";
static const char PVRMMETADATA_WRITER_KEY[] = "writer";
static const char PVRMMETADATA_GROUPING_KEY[] = "grouping";
static const char PVRMMETADATA_TRACKDATA_KEY[] = "track data";
static const char PVRMMETADATA_COMPILATION_KEY[] = "compilation";
static const char PVRMMETADATA_TEMPO_KEY[] = "tempo";
static const char PVRMMETADATA_COVER_KEY[] = "cover";
static const char PVRMMETADATA_DISKDATA_KEY[] = "disk";
static const char PVRMMETADATA_FREEFORMDATA_KEY[] = "free form data";
static const char PVRMMETADATA_CDDBID_KEY[] = "CD identifier";
static const char PVRMMETADATA_LYRICS_KEY[] = "lyrics";
static const char PVRMMETADATA_RANDOM_ACCESS_DENIED_KEY[] = "random-access-denied";
//////////////////////////////////////////////////////////

static const char PVRMMETADATA_TRACKINFO_TYPE_KEY[] = "track-info/type";
static const char PVRMMETADATA_TRACKINFO_TRACKID_KEY[] = "track-info/track-id";
static const char PVRMMETADATA_TRACKINFO_DURATION_KEY[] = "track-info/duration";
static const char PVRMMETADATA_TRACKINFO_BITRATE_KEY[] = "track-info/bit-rate";
static const char PVRMMETADATA_TRACKINFO_SAMPLECOUNT_KEY[] = "track-info/num-samples";
static const char PVRMMETADATA_TRACKINFO_SELECTED_KEY[] = "track-info/selected";

static const char PVRMMETADATA_TRACKINFO_AUDIO_FORMAT_KEY[] = "track-info/audio/format";
static const char PVRMMETADATA_TRACKINFO_AUDIO_NUMCHANNELS_KEY[] = "track-info/audio/channels";
static const char PVRMMETADATA_TRACKINFO_SAMPLERATE_KEY[] = "track-info/sample-rate";
static const char PVRMMETADATA_TRACKINFO_AUDIO_BITS_PER_SAMPLE_KEY[] = "track-info/audio/bits-per-sample";

static const char PVRMMETADATA_TRACKINFO_VIDEO_FORMAT_KEY[] = "track-info/video/format";
static const char PVRMMETADATA_TRACKINFO_VIDEO_WIDTH_KEY[] = "track-info/video/width";
static const char PVRMMETADATA_TRACKINFO_VIDEO_HEIGHT_KEY[] = "track-info/video/height";
static const char PVRMMETADATA_TRACKINFO_VIDEO_PROFILE_KEY[] = "track-info/video/profile";
static const char PVRMMETADATA_TRACKINFO_VIDEO_LEVEL_KEY[] = "track-info/video/level";
static const char PVRMMETADATA_TRACKINFO_FRAME_RATE_KEY[] = "track-info/frame-rate";
static const char PVRMMETADATA_TRACKINFO_TRACK_NUMBER_KEY[] = "track-info/track-number";
static const char PVRMMETADATA_TRACKINFO_NUM_KEY_SAMPLES_KEY[] = "track-info/num-key-samples";

static const char PVRMMETADATA_MAJORBRAND_KEY[] = "mp4ff/major-brand";
static const char PVRMMETADATA_COMPATIBLEBRAND_KEY[] = "mp4ff/compatible-brand";

static const char PVRMMETADATA_SEMICOLON[] = ";";
static const char PVRMMETADATA_TIMESCALE[] = "timescale=";
static const char PVRMMETADATA_INDEX[] = "index=";
static const char PVRMMETADATA_LANG_CODE[] = "iso-639-2-lang=";
static const char PVRMMETADATA_NOT_SOTRABLE[] = "not-storable";
static const char PVRMMETADATA_MAXSIZE[] = "maxsize=";
static const char PVRMMETADATA_REQ_SIZE[] = "reqsize=";
static const char PVRMMETADATA_ORIG_CHAR_ENC[] = "orig-char-enc=";

#define PVMF_RM_MIME_FORMAT_AUDIO_UNKNOWN  "x-pvmf/audio/unknown"
#define PVMF_RM_MIME_FORMAT_VIDEO_UNKNOWN  "x-pvmf/video/unknown"
#define PVMF_RM_MIME_FORMAT_UNKNOWN        "x-pvmf/unknown-media/unknown"

#define MILLISECOND_TIMESCALE (1000)
#define PVMF_RM_MAX_UINT32   (0xffffffffU)

uint32 PVMFRMFFParserNode::GetNumMetadataKeys(char* aQueryKeyString)
{
  
    uint32 num_entries = 0;

    if (aQueryKeyString == NULL)
    {
        // No query key so just return all the available keys
        num_entries = iAvailableMetadataKeys.size();
    }
    else
    {
        // Determine the number of metadata keys based on the query key string provided
        for (uint32 i = 0; i < iAvailableMetadataKeys.size(); i++)
        {
            // Check if the key matches the query key
            if (pv_mime_strcmp(iAvailableMetadataKeys[i].get_cstr(), aQueryKeyString) >= 0)
            {
                num_entries++;
            }
        }
    }

    LOGE ("@@ PVMFRMFFParserNode::GetNumMetadataKeys return %d", num_entries);
    return num_entries;
}


uint32 PVMFRMFFParserNode::GetNumMetadataValues(PVMFMetadataList& aKeyList)
{
    LOGE ("@@ PVMFRMFFParserNode::GetNumMetadataValues aKeyList.size()=%d", aKeyList.size());

    uint32 numvalentries = 0;
    
    if (aKeyList.size() == 0)
    {
        return 0;
    }

    uint32 numkeys = aKeyList.size();
    for (uint32 lcv = 0; lcv < numkeys; lcv++)
    {
        if (oscl_strcmp(aKeyList[lcv].get_cstr(), PVRMMETADATA_DURATION_KEY) == 0)
        {
            // Movie Duration
            // Increment the counter for the number of values found so far
            ++numvalentries;
        }
    }

    LOGE ("@@ PVMFRMFFParserNode::GetNumMetadataValues return %d", numvalentries);

    return numvalentries;
}


PVMFCommandId PVMFRMFFParserNode::GetNodeMetadataKeys(PVMFSessionId aSessionId, PVMFMetadataList& aKeyList, uint32 starting_index, int32 max_entries, char* query_key, const OsclAny* aContext)
{
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommand::Construct(aSessionId, PVMFRMFFPARSER_NODE_CMD_GETNODEMETADATAKEYS, aKeyList, starting_index, max_entries, query_key, aContext);
    return QueueCommandL(cmd);
}


PVMFCommandId PVMFRMFFParserNode::GetNodeMetadataValues(PVMFSessionId aSessionId, PVMFMetadataList& aKeyList, Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, uint32 starting_index, int32 max_entries, const OsclAny* aContext)
{
   
    //LOGE ("[M] PVMFRMFFParserNode::GetNodeMetadataValue() called");
    PVMFRMFFParserNodeCommand cmd;
    cmd.PVMFRMFFParserNodeCommand::Construct(aSessionId, PVMFRMFFPARSER_NODE_CMD_GETNODEMETADATAVALUES, aKeyList, aValueList, starting_index, max_entries, aContext);
    return QueueCommandL(cmd);
}


PVMFStatus PVMFRMFFParserNode::ReleaseNodeMetadataKeys(PVMFMetadataList& , uint32 , uint32)
{
    // Nothing needed-- there's no dynamic allocation in this node's key list
    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::ReleaseNodeMetadataValues(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList,
        uint32 start,
        uint32 end)
{
    if (start > end || aValueList.size() == 0)
    {
        LOGE ("PVMFRMFFParserNode::ReleaseNodeMetadataValues() Invalid start/end index");
        return PVMFErrArgument;
    }

    end = iRMParserNodeMetadataValueCount;

    for (uint32 i = start; i < end; i++)
    {
        char* key = aValueList[i].key;
        if (key != NULL)
        {
            switch (GetValTypeFromKeyString(key))
            {
                case PVMI_KVPVALTYPE_WCHARPTR:
                    if (aValueList[i].value.pWChar_value != NULL)
                    {
                        OSCL_ARRAY_DELETE(aValueList[i].value.pWChar_value);
                        aValueList[i].value.pWChar_value = NULL;
                    }
                    break;

                case PVMI_KVPVALTYPE_CHARPTR:
                    if (aValueList[i].value.pChar_value != NULL)
                    {
                        OSCL_ARRAY_DELETE(aValueList[i].value.pChar_value);
                        aValueList[i].value.pChar_value = NULL;
                    }
                    break;

                case PVMI_KVPVALTYPE_UINT32:
                case PVMI_KVPVALTYPE_FLOAT:
                case PVMI_KVPVALTYPE_BOOL:
                    // No need to free memory for this valtype
                    break;


                case PVMI_KVPVALTYPE_KSV:


                    /*              if (aValueList[i].value.key_specific_value != NULL)
                                     {

                                            if( ((PvmfApicStruct *)aValueList[i].value.key_specific_value)->iGraphicData != NULL)
                                            {
                                                oscl_free(((PvmfApicStruct *)aValueList[i].value.key_specific_value)->iGraphicData);                                            //OSCL_DEFAULT_FREE(((PvmfApicStruct *)aValueKVP.value.key_specific_value)->iGraphicMimeType);
                                                ((PvmfApicStruct *)aValueList[i].value.key_specific_value)->iGraphicData=NULL;
                                            }

                                            OSCL_DELETE(((PvmfApicStruct *)aValueList[i].value.key_specific_value));

                                            aValueList[i].value.key_specific_value=NULL;

                                    }
                    */
                    break;

                default:
                {
                    // Should not get a value that wasn't created from this node
                    OSCL_ASSERT(false);
                }
                break;
            }
        
            OSCL_ARRAY_DELETE(aValueList[i].key);
            aValueList[i].key = NULL;
        }
    }
    LOGE ("PVMFRMFFParserNode::ReleaseNodeMetadataValues() complete");
    return PVMFSuccess;
}


void PVMFRMFFParserNode::PushToAvailableMetadataKeysList(const char* aKeystr, char* aOptionalParam)
{
    if (aKeystr == NULL)
    {
        return;
    }

    if (aOptionalParam)
    {
        iAvailableMetadataKeys.push_front(aKeystr);
        iAvailableMetadataKeys[0] += aOptionalParam;
        LOGE ("PVMFRMFFParserNode::PushToAvailableMetadataKeysList A (%s, %s)", aKeystr, aOptionalParam);
    }

    else
    {
        iAvailableMetadataKeys.push_front(aKeystr);
        LOGE ("PVMFRMFFParserNode::PushToAvailableMetadataKeysList A (%s)", aKeystr);
    }
}



int32 PVMFRMFFParserNode::CountMetaDataKeys()
{
//    MP4FFParserOriginalCharEnc charType;
    int32 NumMetaDataKeysAvailable = 0;

    NumMetaDataKeysAvailable+=1 ; // duration
    
    return NumMetaDataKeysAvailable;
}


PVMFStatus PVMFRMFFParserNode::InitMetaData()
{
//    MP4FFParserOriginalCharEnc charType;

    int32 AvailableMetaDataKeysCount = CountMetaDataKeys();

    if (iRmParser)
    {
	 uint64 movieDuration = rm_parser_get_duration(iRmParser);
        LOGE ("@@ PVMFRMFFParserNode::InitMetaData() movieDuration=%lld", movieDuration);

        PushToAvailableMetadataKeysList(PVRMMETADATA_DURATION_KEY);

        uint32 duration = Oscl_Int64_Utils::get_uint64_lower32(movieDuration);
        
        // Intimate the Duration info available to the engine through Informational Event.
        CreateDurationInfoMsg(duration);
    }
    
    
    return PVMFSuccess;
}

PVMFStatus
PVMFRMFFParserNode::DoGetMetadataKeys(PVMFRMFFParserNodeCommand& aCmd)
{
    return (CompleteGetMetadataKeys(aCmd));
}


PVMFStatus PVMFRMFFParserNode::CompleteGetMetadataKeys(PVMFRMFFParserNodeCommand& aCmd)
{

    PVMFMetadataList* keylistptr = NULL;
    uint32 starting_index;
    int32 max_entries;
    char* query_key = NULL;

    aCmd.PVMFRMFFParserNodeCommand::Parse(keylistptr, starting_index, max_entries, query_key);

    // Check parameters
    if (keylistptr == NULL)
    {
        // The list pointer is invalid
        return PVMFErrArgument;
    }

    if ((starting_index > (iAvailableMetadataKeys.size() - 1)) || max_entries == 0)
    {
        // Invalid starting index and/or max entries
        return PVMFErrArgument;
    }

    // Copy the requested keys
    uint32 num_entries = 0;
    int32 num_added = 0;
    uint32 lcv = 0;
  
    for (lcv = 0; lcv < iAvailableMetadataKeys.size(); lcv++)
    {
        if (query_key == NULL)
        {
            // No query key so this key is counted
            ++num_entries;
            if (num_entries > starting_index)
            {
                // Past the starting index so copy the key
                PVMFStatus status = PushValueToList(iAvailableMetadataKeys, keylistptr, lcv);
                if (PVMFErrNoMemory == status)
                {
                    return status;
                }
                num_added++;
            }
        }
        else
        {
            // Check if the key matche the query key
            if (pv_mime_strcmp(iAvailableMetadataKeys[lcv].get_cstr(), query_key) >= 0)
            {
                // This key is counted
                ++num_entries;
                if (num_entries > starting_index)
                {
                    // Past the starting index so copy the key
                    PVMFStatus status = PushValueToList(iAvailableMetadataKeys, keylistptr, lcv);
                    if (PVMFErrNoMemory == status)
                    {
                        return status;
                    }
                    num_added++;
                }
            }
        }

        // Check if max number of entries have been copied
        if (max_entries > 0 && num_added >= max_entries)
        {
            break;
        }
    }
    return PVMFSuccess;
}


PVMFStatus PVMFRMFFParserNode::DoGetMetadataValues(PVMFRMFFParserNodeCommand& aCmd)
{

    PVMFMetadataList* keylistptr_in = NULL;
    PVMFMetadataList* keylistptr = NULL;
    OSCL_wHeapString<OsclMemAllocator> valuestring = NULL;
    Oscl_Vector<PvmiKvp, OsclMemAllocator>* valuelistptr = NULL;
    uint32 starting_index;
    int32 max_entries;
//    MP4FFParserOriginalCharEnc charType = ORIGINAL_CHAR_TYPE_UNKNOWN;
    uint16 iLangCode = 0;
    aCmd.PVMFRMFFParserNodeCommand::Parse(keylistptr_in,
                                           valuelistptr,
                                           starting_index,
                                           max_entries);

    // Check the parameters
    if (keylistptr_in == NULL || valuelistptr == NULL)
    {
        return PVMFErrArgument;
    }

    keylistptr = keylistptr_in;
    //If numkeys is one, just check to see if the request
    //is for ALL metadata
    if (keylistptr_in->size() == 1)
    {
        if (oscl_strncmp((*keylistptr)[0].get_cstr(),
                         PVRM_ALL_METADATA_KEY,
                         oscl_strlen(PVRM_ALL_METADATA_KEY)) == 0)
        {
            //use the complete metadata key list
            keylistptr = &iAvailableMetadataKeys;
        }
    }
    uint32 numkeys = keylistptr->size();

    if (starting_index > (numkeys - 1) || numkeys <= 0 || max_entries == 0)
    {
        // Don't do anything
        return PVMFErrArgument;
    }

    uint32 numvalentries = 0;
    int32 numentriesadded = 0;
    uint32 lcv = 0;

    if (iRmParser)
    {
    	   for (lcv = 0; lcv < numkeys; lcv++)
          {
          	 int32 leavecode = 0;
            	 PvmiKvp KeyVal;
               KeyVal.key = NULL;
               KeyVal.value.pWChar_value = NULL;
               KeyVal.value.pChar_value = NULL;
               int32 idx = 0;
               char orig_char_enc[2][7] = {"UTF-8", "UTF-16"};

               // const char *x = (*keylistptr)[lcv].get_cstr();
               bool IsMetadataValAddedBefore = false;
            
               uint64 movieDuration = rm_parser_get_duration(iRmParser);
               if (oscl_strcmp((*keylistptr)[lcv].get_cstr(), PVRMMETADATA_DURATION_KEY) == 0 && movieDuration > (uint64)0)
               {
                   // Movie Duration
                   // Increment the counter for the number of values found so far
                   ++numvalentries;

                   // Create a value entry if past the starting index
                   if (numvalentries > starting_index)
                   {
                       uint32 duration = Oscl_Int64_Utils::get_uint64_lower32(movieDuration);
                       char timescalestr[20];
                       oscl_snprintf(timescalestr, 20, ";%s%d", PVRMMETADATA_TIMESCALE, 1000);
                       timescalestr[19] = '\0';
                       PVMFStatus retval = PVMFCreateKVPUtils::CreateKVPForUInt32Value(KeyVal, PVRMMETADATA_DURATION_KEY, duration, timescalestr);
                       if (retval != PVMFSuccess && retval != PVMFErrArgument)
                       {
                           break;
                       }
                   }
           	  }

                 // Add the KVP to the list if the key string was created
           	   if ((KeyVal.key != NULL) && (!IsMetadataValAddedBefore))
	          {
                	leavecode = AddToValueList(*valuelistptr, KeyVal);
                	if (leavecode != 0)
                	{
                        switch (GetValTypeFromKeyString(KeyVal.key))
                        {
                            case PVMI_KVPVALTYPE_CHARPTR:
                            if (KeyVal.value.pChar_value != NULL)
                            {
                                OSCL_ARRAY_DELETE(KeyVal.value.pChar_value);
                                KeyVal.value.pChar_value = NULL;
                            }
                            break;

                            case PVMI_KVPVALTYPE_WCHARPTR:
                            if (KeyVal.value.pWChar_value != NULL)
                            {
                                OSCL_ARRAY_DELETE(KeyVal.value.pWChar_value);
                                KeyVal.value.pWChar_value = NULL;
                            }
                            break;

                        default:
                            // Add more case statements if other value types are returned
                            break;
                    }

                    OSCL_ARRAY_DELETE(KeyVal.key);
                    KeyVal.key = NULL;
                }
                else
                {
                    // Increment the counter for number of value entries added to the list
                    ++numentriesadded;
                }

                // Check if the max number of value entries were added
                if (max_entries > 0 && numentriesadded >= max_entries)
                {
                    // Maximum number of values added so break out of the loop
                    //return PVMFSuccess;
                    break;
                }
            }
    	  }
            
	       iRMParserNodeMetadataValueCount = (*valuelistptr).size();
	       LOGE ("@@ DoGetMetadataValues iRMParserNodeMetadataValueCount=%d", iRMParserNodeMetadataValueCount);
    	}
            
    return PVMFSuccess;
}

void PVMFRMFFParserNode::CompleteGetMetaDataValues()
{
    PVMFMetadataList* keylistptr = NULL;
    Oscl_Vector<PvmiKvp, OsclMemAllocator>* valuelistptr = NULL;
    uint32 starting_index;
    int32 max_entries;

    iCurrentCommand.front().PVMFRMFFParserNodeCommand::Parse(keylistptr, valuelistptr, starting_index, max_entries);

    for (uint32 i = 0; i < (*valuelistptr).size(); i++)
    {
    }

    CommandComplete(iCurrentCommand,
                    iCurrentCommand.front(),
                    PVMFSuccess);
}

int32 PVMFRMFFParserNode::AddToValueList(Oscl_Vector<PvmiKvp, OsclMemAllocator>& aValueList, PvmiKvp& aNewValue)
{
    int32 leavecode = 0;
    OSCL_TRY(leavecode, aValueList.push_back(aNewValue));
    return leavecode;
}

void PVMFRMFFParserNode::DeleteAPICStruct(PvmfApicStruct*& aAPICStruct)
{
    OSCL_ARRAY_DELETE(aAPICStruct->iGraphicData);
    OSCL_DELETE(aAPICStruct);
    aAPICStruct = NULL;
}

PVMFStatus PVMFRMFFParserNode::GetIndexParamValues(const char* aString, uint32& aStartIndex, uint32& aEndIndex)
{
    // This parses a string of the form "index=N1...N2" and extracts the integers N1 and N2.
    // If string is of the format "index=N1" then N2=N1

    if (aString == NULL)
    {
        return PVMFErrArgument;
    }

    // Go to end of "index="
    char* n1string = (char*)aString + 6;

    PV_atoi(n1string, 'd', oscl_strlen(n1string), aStartIndex);

    const char* n2string = oscl_strstr(aString, _STRLIT_CHAR("..."));

    if (n2string == NULL)
    {
        aEndIndex = aStartIndex;
    }
    else
    {
        // Go to end of "index=N1..."
        n2string += 3;

        PV_atoi(n2string, 'd', oscl_strlen(n2string), aEndIndex);
    }

    return PVMFSuccess;
}

void PVMFRMFFParserNode::getLanguageCode(uint16 langcode, int8 *LangCode)
{
    //ISO-639-2/T 3-char Lang Code
    oscl_memset(LangCode, 0, 4);
    LangCode[0] = 0x60 + ((langcode >> 10) & 0x1F);
    LangCode[1] = 0x60 + ((langcode >> 5) & 0x1F);
    LangCode[2] = 0x60 + ((langcode) & 0x1F);
}

void PVMFRMFFParserNode::CreateDurationInfoMsg(uint32 adurationms)
{
    int32 leavecode = 0;
    PVMFDurationInfoMessage* eventmsg = NULL;
    OSCL_TRY(leavecode, eventmsg = OSCL_NEW(PVMFDurationInfoMessage, (adurationms)));
    PVMFNodeInterface::ReportInfoEvent(PVMFInfoDurationAvailable, NULL, OSCL_STATIC_CAST(PVInterface*, eventmsg));
    if (eventmsg)
    {
        eventmsg->removeRef();
    }
}

PVMFStatus PVMFRMFFParserNode::PushKVPToMetadataValueList(Oscl_Vector<PvmiKvp, OsclMemAllocator>* aVecPtr, PvmiKvp& aKvpVal)
{
    if (aVecPtr == NULL)
    {
        return PVMFErrArgument;
    }
    int32 leavecode = 0;
    OSCL_TRY(leavecode, aVecPtr->push_back(aKvpVal););
    if (leavecode != 0)
    {
        OSCL_ARRAY_DELETE(aKvpVal.key);
        aKvpVal.key = NULL;
        return PVMFErrNoMemory;
    }
    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::CreateNewArray(uint32** aTrackidList, uint32 aNumTracks)
{
    int32 leavecode = 0;
    OSCL_TRY(leavecode, *aTrackidList = OSCL_ARRAY_NEW(uint32, aNumTracks););
    OSCL_FIRST_CATCH_ANY(leavecode, return PVMFErrNoMemory;);
    return PVMFSuccess;
}

PVMFStatus PVMFRMFFParserNode::PushValueToList(Oscl_Vector<OSCL_HeapString<OsclMemAllocator>, OsclMemAllocator> &aRefMetaDataKeys, PVMFMetadataList *&aKeyListPtr, uint32 aLcv)
{
    int32 leavecode = 0;
    OSCL_TRY(leavecode, aKeyListPtr->push_back(aRefMetaDataKeys[aLcv]));
    OSCL_FIRST_CATCH_ANY(leavecode, LOGE ("PVMFRMFFParserNode::PushValueToList() Memory allocation failure when copying metadata key"); return PVMFErrNoMemory);
    return PVMFSuccess;
}
