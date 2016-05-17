#ifndef PAST_LINEAR_REGRESSION_H_
#define PAST_LINEAR_REGRESSION_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <gtest/gtest.h>

#define CILK_DOUBLE cilk::reducer< cilk::op_add<double> >

int linear_regression(std::vector<double> *, std::vector<double> *, double*, double*);
#endif
