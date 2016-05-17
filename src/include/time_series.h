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
    double mean;
    double std_dev;
    public:
        std::vector<double> values;
        TimeSeries();
        TimeSeries(std::vector<double> vals);
        ~TimeSeries();
        void add_data(double);
        void extend(std::vector<double> vals);
        double compute_mean();
        double compute_mean_parallel();
        double compute_stddev();
        double compute_stddev(int);
        double compute_volatility();
        std::vector<double> compute_returns();
        std::vector<double> compute_returns(int);
        double compute_skewness(int);
        double compute_kurtosis(int);
};
#endif
