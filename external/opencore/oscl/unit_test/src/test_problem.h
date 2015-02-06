
#ifndef TEST_PROBLEM_H
#define TEST_PROBLEM_H

#include "unit_test_common.h"



//$Id
//this is a test "problem" -- a note when a test failed or erred
class test_problem
{
    protected:
        //protected data members
        //line number of the error, if appropriate
        long m_line_number;
        //filename of the error, if appropriate
        _STRING m_filename;
        //additional message as appropriate
        _STRING m_message;
    public:
        //construction
        test_problem(const _STRING& filename, long line_number, const _STRING& message);
        test_problem(const test_problem& rhs);
        test_problem& operator= (const test_problem& rhs);
        //data access
        long line_number(void) const;
        const _STRING& filename(void) const;
        const _STRING& message(void) const;

};

#endif
