

#include "oscl_base.h"
#include "oscl_registry_client_impl.h"

#include "oscl_error_codes.h"

#include "osclconfig_proc.h"
#if (OSCL_HAS_SINGLETON_SUPPORT)
//1st choice implementation-- uses Oscl Singleton

//nothing needed.  client goes directly to server impl.

#else //OSCL_HAS_ ...
//3rd choice implementation

//non-functional stubs are implemented in the header-- nothing needed here.

#endif //oscl config





