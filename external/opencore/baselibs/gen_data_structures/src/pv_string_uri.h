

// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     pv_string_uri

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =




#ifndef PV_STRING_URI_H_INCLUDE
#define PV_STRING_URI_H_INCLUDE



#include "oscl_stdstring.h"
#include "oscl_mem_basic_functions.h"
#include "oscl_mem.h"
#include  "oscl_base_macros.h"
#include "oscl_base.h"

#define MAX_FULL_REQUEST_SIZE 4000
#define PERSENTAGE '%'





class PVStringUri
{

    private:


    public:
        /**
        * PersentageToEscapedEncoding utility function provides the Persentage Pct-Encoding to %25
        * If Two Digit after % will not match in Legal Character List Of RFC-3896
        * @param [in]/[Out] character Pointer; The complete URI
        * @param [out] URL Lenght
        *@return true if successful, otherwise false.
        */
        OSCL_IMPORT_REF static bool  PersentageToEscapedEncoding(char*, uint32 &);

        /**
        * IllegalCharactersToEscapedEncoding utility function provides the EscapedEncoding
        * for illegal character , Which are not fall in URI-Legal Character List Of RFC-3896
        * @param [in]/[Out] character Pointer; The complete URI
        * @param [out] URL Lenght
        *@return true if successful, otherwise false.
        */

        OSCL_IMPORT_REF static bool  IllegalCharactersToEscapedEncoding(char*, uint32 &);


        /**
        * DecimalToHexConverter utility function provides the Decimal To Hexa.
        *
        * @param [in] unsigned character Pointer.
        * @param [out] Return The Hexa Value for a character.
        * @return return Hexa Value for a character.
        */

        OSCL_IMPORT_REF static unsigned char* DecimalToHexConverter(unsigned char aNumberIn, uint32 & aUrlMaxConvertLenght);


    protected:



};



#endif // PV_STRING_URI_H_INCLUDE
/*! @} */

