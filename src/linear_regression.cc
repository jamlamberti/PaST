#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <gtest/gtest.h>

#define CILK_DOUBLE cilk::reducer< cilk::op_add<double> >

int linear_regression(std::vector<double> *x, std::vector<double> *y, double* m, double* b)
{
    // based on http://stackoverflow.com/a/19040841
    CILK_DOUBLE cilk_sum_x(0.0);
    CILK_DOUBLE cilk_sum_y(0.0);
    CILK_DOUBLE cilk_sum_xy(0.0);
    CILK_DOUBLE cilk_sum_xx(0.0);
    CILK_DOUBLE cilk_sum_yy(0.0);

    double sum_x, sum_y, sum_xy, sum_xx, sum_yy;

    int end = x->size();
    if (end != y->size())
    {
        #ifndef TESTING
        std::cout << " [-] x and y must be the same size!" << std::endl;
        #endif
        return 1;
    }

    cilk_for(int i = 0; i < end; i++)
    {
        *cilk_sum_x += x->at(i);
        *cilk_sum_xx += pow(x->at(i), 2);
        *cilk_sum_xy += x->at(i) * y->at(i);
        *cilk_sum_y += y->at(i);
        *cilk_sum_yy += pow(y->at(i), 2);
    }
    sum_x  = cilk_sum_x.get_value();
    sum_xx = cilk_sum_xx.get_value();
    sum_xy = cilk_sum_xy.get_value();
    sum_yy = cilk_sum_yy.get_value();
    sum_y  = cilk_sum_y.get_value();
    double denom = end*sum_xx - pow(sum_x, 2);
    if (denom == 0.0)
    {
        #ifndef TESTING
        std::cout << " [-] Singular matrix, cannot compute linear regression" << std::endl;
        #endif
        return 1;
    }

    *m = (end*sum_xy - sum_x*sum_y)/denom;
    *b = (sum_y*sum_xx - sum_x*sum_xy)/denom;
    return 0;
}

TEST(LinearRegression, DifferentSizedVectors)
{
    std::vector<double> x {0, 1, 2, 3};
    std::vector<double> y {0, 1, 2};
    double m = 0;
    double b = 0;
    int res = linear_regression(&x, &y, &m, &b);
    // Make sure it failed!
    EXPECT_EQ(res, 1);
}

TEST(LinearRegression, SingularMatrix)
{
    std::vector<double> x {1};
    std::vector<double> y {1};
    double m = 0;
    double b = 0;
    int res = linear_regression(&x, &y, &m, &b);
    // It should fail!
    EXPECT_EQ(res, 1);
}

TEST(LinearRegression, PerfectFit)
{
    std::vector<double> x {0, 1, 2, 3, -1, -5};
    std::vector<double> y {-1, 0, 1, 2, -2, -6};
    double m = 0;
    double b = 0;
    int res = linear_regression(&x, &y, &m, &b);
    EXPECT_EQ(m, 1);
    EXPECT_EQ(b, -1);
}

TEST(LinearRegression, ImperfectFit)
{
    std::vector<double> x {0, 1, 2, 3, -1, -5, 6};
    std::vector<double> y {-1, 0, 1, 2, -2, -6, 6};
    double m = 0;
    double b = 0;
    int res = linear_regression(&x, &y, &m, &b);
    EXPECT_LE( abs(m-1.0726), 0.0001);
    EXPECT_LT( abs(b+0.9194), 0.0001);
}
