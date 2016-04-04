#ifndef PAST_RISK_MEASURES_H_
#define PAST_RISK_MEASURES_H_
#include <iostream>
#include <vector>
#include <cmath>
#include "time_series.h"
#include <gtest/gtest.h>

class RiskMeasures
{
    TimeSeries ts;
    public:
        RiskMeasures(TimeSeries);
        ~RiskMeasures();
        double value_at_risk(int, double);

};


#endif
