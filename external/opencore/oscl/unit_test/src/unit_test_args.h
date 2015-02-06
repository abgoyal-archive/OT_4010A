
#ifndef __UNIT_TEST_TEST_ARGS__
#define __UNIT_TEST_TEST_ARGS__

#include "unit_test_common.h"

class cmd_line
{
    public:

        virtual ~cmd_line() {}

        virtual int get_count(void) = 0;

        virtual void setup(int count, oscl_wchar** cmd_argv) = 0;
        virtual void setup(int count, char** cmd_argv) = 0;


        virtual void get_arg(int n, char* &arg) = 0;


        virtual void get_arg(int n, oscl_wchar* &arg) = 0;

        virtual bool is_wchar(void) = 0;
};



class cmd_line_linux : public cmd_line
{
    private:
        char** argv;
        int argc;

    public:
        void setup(int count, oscl_wchar** cmd_argv);
        void setup(int count, char** cmd_argv);

        int get_count(void);

        void get_arg(int n, char* &arg);
        void get_arg(int n, oscl_wchar *&arg);

        bool is_wchar(void);
};



#endif //__UNIT_TEST_TEST_ARGS__
