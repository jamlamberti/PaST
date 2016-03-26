#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <cilk/cilk.h>
#include <cilk/reducer_opadd.h>
#include <gtest/gtest.h>

class time_series
{
    std::vector<double> values;
    double ts_mean;
    double ts_std;
    public:
        time_series()
        {
            // Do nothing for now
        }

        time_series(std::vector<double> vals)
        {
            values = std::move(vals);
        }

        ~time_series()
        {
            // Do nothing for now
        }

        double mean()
        {
            cilk::reducer< cilk::op_add<double> > parallel_sum(0);
            auto end = values.end();
            cilk_for(auto i = values.begin(); i < end; i++)
            {
                *parallel_sum += *i;
            }
            ts_mean = parallel_sum.get_value()/values.size();
            return ts_mean;
        }

        double stddev()
        {
            cilk::reducer< cilk::op_add<double> > square_error(0);
            auto end = values.end();
            cilk_for(auto i = values.begin(); i < end; i++)
            {
                *square_error += pow(*i-ts_mean, 2);
            }
            ts_std = sqrt(square_error.get_value()/values.size());
            return ts_std;
        }

};

TEST(TimeSeriesStats, ComputeMeanStd)
{
    std::vector<double> tc {0.15330792502278168, 0.9288688131475954, 0.6467927448900687, 0.8391664129724394, 0.04798762394687839, 0.6393277709551051, 0.2727015569864173, 0.8360877258225698, 0.4975557719102256, 0.1608764499225922};
    time_series ts(tc);
    ASSERT_LE(abs(ts.mean() - 0.5022), 0.0001);
    ASSERT_LE(abs(ts.stddev() - 0.3072), 0.0001);
}
