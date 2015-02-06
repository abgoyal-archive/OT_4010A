
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ N A M E S T R I N G

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef OSCL_NAMESTRING_H_INCLUDED
#define OSCL_NAMESTRING_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

template<int __len>
class OsclNameString
{
    public:
        OsclNameString()
        {
            Set("");
        }

        OsclNameString(const char a[])
        {
            Set((uint8*)a);
        }

        OsclNameString(uint8* a)
        {
            Set(a);
        }

        /**
        * Set the string to the input value.  The string
        * will be truncated to fit the storage class and
        * automatically null-terminated.
        *
        * @param a (input param): null-terminated character
        *    string.
        */
        void Set(uint8* a)
        {
            for (int i = 0; i < __len; i++)
                iStr[i] = '\0';
            if (a)
            {
                for (int i = 0; i < __len - 1; i++)
                {
                    iStr[i] = a[i];
                    if (a[i] == '\0')
                        return;
                }
            }
        }

        void Set(const char a[])
        {
            Set((uint8*)a);
        }

        uint8* Str()const
        {
            return (uint8*)&iStr[0];
        }

        int32 MaxLen()const
        {
            return __len;
        }

    private:
        uint8 iStr[__len];
};
#endif

/*! @} */
