#include "time_series.h"

TimeSeries::TimeSeries()
{
    // Do nothing for now
}

TimeSeries::TimeSeries(std::vector<double> vals)
{
    values = std::move(vals);
    mean = 0.0;
    std_dev = 0.0;
    

}

TimeSeries::~TimeSeries()
{
    // Do nothing for now
}

double TimeSeries::compute_mean()
{
    cilk::reducer< cilk::op_add<double> > parallel_sum(0.0);
    auto end = values.end();
    cilk_for(auto i = values.begin(); i < end; i++)
    {
        *parallel_sum += *i;
    }
    mean = parallel_sum.get_value()/values.size();
    return mean;
}

double TimeSeries::compute_stddev()
{
    if (mean == 0.0)
    {
        TimeSeries::compute_mean();
    }

    cilk::reducer< cilk::op_add<double> > square_error(0);
    auto end = values.end();
    cilk_for(auto i = values.begin(); i < end; i++)
    {
        *square_error += pow(*i-mean, 2);
    }
    std_dev = sqrt(square_error.get_value()/values.size());
    return std_dev;
}

TEST(TimeSeriesStats, ComputeMeanStd)
{
    std::vector<double> tc {0.15330792502278168, 0.9288688131475954, 0.6467927448900687, 0.8391664129724394, 0.04798762394687839, 0.6393277709551051, 0.2727015569864173, 0.8360877258225698, 0.4975557719102256, 0.1608764499225922};
    TimeSeries ts(tc);
    EXPECT_LE(abs(ts.compute_mean() - 0.5022), 0.0001);
    EXPECT_LE(abs(ts.compute_stddev() - 0.3072), 0.0001);
}
