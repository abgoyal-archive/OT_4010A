
#include "unit_test_args.h"



void cmd_line_linux::setup(int , oscl_wchar**)
{
    argc = 0;
    argv = NULL;
}

void cmd_line_linux::setup(int count, char** cmd_argv)
{
    argc = count;
    argv = cmd_argv;
}


int cmd_line_linux::get_count(void)
{
    return argc;
}

void cmd_line_linux::get_arg(int n, char* &arg)
{
    if (n > argc)
        arg = NULL;
    else
        arg = argv[n];
}

void cmd_line_linux::get_arg(int , oscl_wchar *&arg)
{
    arg = NULL;
}

bool cmd_line_linux::is_wchar(void)
{
    return false;
}

