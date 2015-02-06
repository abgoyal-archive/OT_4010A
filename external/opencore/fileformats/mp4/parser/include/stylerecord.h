
#ifndef STYLERECORD_H_INCLUDED
#define STYLERECORD_H_INCLUDED

#ifndef ATOMUTILS_H_INCLUDED
#include "atomutils.h"
#endif

class StyleRecord
{

    public:
        StyleRecord(MP4_FF_FILE *fp);  // Default constructor
        StyleRecord(uint8 *&buf);  // Default constructor
        virtual ~StyleRecord();  // Destructor

        uint16 getStartChar()
        {
            return _startChar;
        }

        uint16 getEndChar()
        {
            return _endChar;
        }

        uint16 getFontID()
        {
            return _fontID;
        }

        uint8 getFontStyleFlags()
        {
            return _fontStyleFlags;
        }

        uint8 getfontSize()
        {
            return _fontSize;
        }

        uint8 *getTextColourRGBA()
        {
            return _pRGBA;
        }

        bool MP4Success()
        {
            return _success;
        }

        int32 GetMP4Error()
        {
            return _mp4ErrorCode;
        }

    private:
        uint16 _startChar;
        uint16 _endChar;
        uint16 _fontID;
        uint8  _fontStyleFlags;
        uint8  _fontSize;
        uint8 *_pRGBA;

        bool  _success;
        int32 _mp4ErrorCode;
};

#endif // STYLERECORD_H_INCLUDED


