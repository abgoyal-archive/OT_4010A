
#ifndef TEXT_TEST_INTERPRETER_H
#define TEXT_TEST_INTERPRETER_H

#ifndef TEST_RESULT_H
#include "test_result.h"
#endif


//a simple interpreter which outputs a result of testing
class text_test_interpreter
{
    protected:
        //partial interpretations
        _STRING header(const test_result& result) const;
        _STRING footer(const test_result& result) const;
        _STRING successes(const test_result& result) const;
        _STRING failures(const test_result& result) const;
        _STRING errors(const test_result& result) const;
        _STRING problem_vector_string(const _VECTOR(test_problem, unit_test_allocator)&
                                      vect) const;
        _STRING problem_string(const test_problem& problem) const;
    public:
        //returns the interpretation of a test result
        _STRING interpretation(const test_result& result_to_interpret) const;
};

#endif
