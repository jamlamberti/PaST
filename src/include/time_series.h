#ifndef PAST_TIME_SERIES_H_
#define PAST_TIME_SERIES_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <gtest/gtest.h>

class TimeSeries
{
    std::vector<double> values;
    double mean;
    double std_dev;
    public:
        TimeSeries();
        TimeSeries(std::vector<double> vals);
        ~TimeSeries();
        double compute_mean();
        double compute_stddev();
};
#endif
