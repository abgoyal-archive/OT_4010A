
#ifndef TEST_POINT_H
#define TEST_POINT_H

#ifndef TEST_CASE_H
#include "test_case.h"
#endif
#ifndef POINT_H
#include "point.h"
#endif

class point_test : public test_case
{
    protected:
        int_point top_left_point;
        int_point bottom_right_point;
    public:
        virtual void set_up(void);
};

class point_test_suite : public test_case
{
    public:
        point_test_suite(void);
        point_test_suite(char**, int);

};

#endif
