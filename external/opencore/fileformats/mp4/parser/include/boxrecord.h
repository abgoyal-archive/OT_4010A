
#ifndef BOXRECORD_H_INCLUDED
#define BOXRECORD_H_INCLUDED

#ifndef ATOMUTILS_H_INCLUDED
#include "atomutils.h"
#endif

class BoxRecord
{

    public:
        BoxRecord(MP4_FF_FILE *fp);  // Default constructor
        BoxRecord(uint8 *&buf);  // Default constructor
        virtual ~BoxRecord() {};  // Destructor

        int16 getBoxTop()
        {
            return _top;
        }

        int16 getBoxLeft()
        {
            return _left;
        }

        int16 getBoxBottom()
        {
            return _bottom;
        }

        int16 getBoxRight()
        {
            return _right;
        }

        bool  MP4Success()
        {
            return _success;
        }

        int32 GetMP4Error()
        {
            return _mp4ErrorCode;
        }

    private:
        int16 _top;
        int16 _left;
        int16 _bottom;
        int16 _right;

        bool  _success;
        int32 _mp4ErrorCode;
};

#endif // BOXRECORD_H_INCLUDED

