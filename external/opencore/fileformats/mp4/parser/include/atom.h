


#ifndef ATOM_H_INCLUDED
#define ATOM_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef PARENTABLE_H_INCLUDED
#include "parentable.h"
#endif
#ifndef RENDERABLE_H_INCLUDED
#include "renderable.h"
#endif
#ifndef ISUCCEEDFAIL_H_INCLUDED
#include "isucceedfail.h"
#endif
#ifndef OSCL_BASE_H_INCLUDE
#include "oscl_base.h"
#endif

#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif

#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif

#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif

#ifndef PVLOGGER_H_INCLUDED
#include "pvlogger.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H
#include "oscl_mem_basic_functions.h"
#endif

class MP4_FF_FILE;
//using namespace std;

//add atom size, so there will be no dead loop when skip sth in atom field
#define DEFAULT_ATOM_SIZE 8 //need to change when they use various size field

class Atom : public Parentable, public Renderable, public ISucceedFail
{

    public:
        Atom(uint32 type);
        Atom(MP4_FF_FILE *fp); // file pointer constructor
        Atom(MP4_FF_FILE *fp, uint32 size, uint32 type); // file pointer constructor
        Atom(uint8* &buf);
        virtual ~Atom();

        // Member get methods
        virtual uint32 getSize() const
        {
            return _size;
        }
        uint32 getType() const
        {
            return _type;
        }
        virtual uint32 getDefaultSize() const;//tag+size field length

    protected:
        uint32 _size; // 4 (32bits)
        uint32 _type; // 4 (32bits)

    private:


};


#endif // ATOM_H_INCLUDED

