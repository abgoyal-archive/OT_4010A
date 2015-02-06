


#include "osclconfig.h"
#include "test_case_file_io.h"
#include "text_test_interpreter.h"

#include <utils/Log.h>
#undef  LOG_TAG
#define LOG_TAG "oscl_file_io_test"



int local_main(FILE* filehandle, cmd_line* command_line)
{
    FILE* file = filehandle;

    file_io_test_suite file_io_test;
    file_io_test.run_test();

    text_test_interpreter interpreter;

    fprintf(file, "%s", interpreter.interpretation(file_io_test.last_result()).c_str());
    LOGI("%s", interpreter.interpretation(file_io_test.last_result()).c_str());
    
    return 0;
}
