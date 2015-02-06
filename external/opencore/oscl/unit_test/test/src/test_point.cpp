
#include "osclconfig.h"
#include "test_case_point.h"
#include "text_test_interpreter.h"

#include <iostream>

int
int local_main(FILE* filehandle, cmd_line* command_line)
{
    point_test_suite pnt_test(argv, argc);
    pnt_test.run_test();
    text_test_interpreter interpreter;

    std::cout << interpreter.interpretation(pnt_test.last_result());
}
