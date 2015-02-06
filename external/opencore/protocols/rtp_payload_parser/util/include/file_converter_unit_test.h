
#ifndef FILE_CONVERTER_TEST_H
#define FILE_CONVERTER_TEST_H


#include "test_case.h"
#include "oscl_types.h"
#include "oscl_string_containers.h"
#include "text_test_interpreter.h"
#include "oscl_vector.h"
#include "oscl_linked_list.h"
#include "rtp_file_converter.h"





class rtp_file_converter_test : public test_case
{
    public:
        virtual void setup(void) {};

        int convert(cmd_line* cmd_line_ptr);

        virtual void test(void);

        rtp_file_converter_test();
        ~rtp_file_converter_test();


        octet* buffer;
        int index;

};


class rtp_file_converter_test_suite : public test_case
{
    public:
        rtp_file_converter_test_suite(cmd_line* cmd_line_ptr)
        {
            iPtrCommandLine = cmd_line_ptr;
        }

        void add_test_case(rtp_file_converter_test *testcase)
        {
            testcase->SetCommandLine(iPtrCommandLine);
            adopt_test_case(testcase);
        }

    private:
        cmd_line* iPtrCommandLine;

};


#endif // FILE_CONVERTER_TEST_H

