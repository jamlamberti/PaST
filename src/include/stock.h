#ifndef PAST_STOCK_H_
#define PAST_STOCK_H_
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <fstream>
#include <gtest/gtest.h>
#include "time_series.h"

class Stock
{
    std::string ticker;
    TimeSeries* ts;
    bool load_data(std::string);

    public:
        Stock(std::string, std::string);
        ~Stock();

};
#endif
