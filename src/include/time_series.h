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
        void add_data(double);
        void extend(std::vector<double> vals);
        double compute_mean();
        double compute_stddev();
};
#endif
