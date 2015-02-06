

#ifndef AAC_MEDIAINFO_H
#define AAC_MEDIAINFO_H

#include "sdp_memory.h"
#include "media_info.h"


// NOTE.... this information really needs to exist for only one media object...
// i.e., there is no reason to have arrays of these fields, since we have one of these
// mediaInfo elements for each media object... the trick is how to get these from the SDP
// file into each of the media elements...



class aac_mediaInfo : public mediaInfo
{
    private:
        int profileLevelID;
        OSCL_HeapString<SDPParserAlloc> lang;
        int numSampleEntries;

    public:
        aac_mediaInfo()
        {
            lang = NULL;
            profileLevelID = -1;
        };
        aac_mediaInfo(const aac_mediaInfo & pSource) : mediaInfo(pSource)
        {
            setLang(pSource.lang);
            setProfileLevelID(pSource.profileLevelID);
        }

        ~aac_mediaInfo()
        {
        };
        inline void setLang(char* language)
        {
            lang = language;
        };
        inline void setLang(const OSCL_HeapString<SDPParserAlloc>& language)
        {
            lang = language;
        };
        inline void setLang(const OsclMemoryFragment memFrag)
        {
            lang.set((const char*)(memFrag.ptr), memFrag.len);
        };
        inline void setProfileLevelID(int pID)
        {
            profileLevelID = pID;
        };

        inline void setNumSampleEntries(int inNumSampleEntries)
        {
            numSampleEntries = inNumSampleEntries;
        };


        inline const char *getLang()
        {
            return lang.get_cstr();
        };
        inline int getProfileLevelID()
        {
            return profileLevelID;
        };
        inline int getNumSampleEntries()
        {
            return numSampleEntries;
        };

};
#endif
