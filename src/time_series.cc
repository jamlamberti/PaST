#include "time_series.h"

TimeSeries::TimeSeries()
{
    mean = 0.0;
    std_dev = 0.0;
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

void TimeSeries::add_data(double data_point)
{
    values.push_back(data_point);
}

void TimeSeries::extend(std::vector<double> vals)
{
    values.reserve(values.size() + vals.size());
    values.insert(values.end(), vals.begin(), vals.end());
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
    return compute_stddev(0);
}

double TimeSeries::compute_stddev(int ddof)
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
    std_dev = sqrt(square_error.get_value()/(values.size()-ddof));
    return std_dev;
}

TEST(TimeSeriesStats, ComputeMeanStd)
{
    std::vector<double> tc {0.15330792502278168, 0.9288688131475954, 0.6467927448900687, 0.8391664129724394, 0.04798762394687839, 0.6393277709551051, 0.2727015569864173, 0.8360877258225698, 0.4975557719102256, 0.1608764499225922};
    TimeSeries ts(tc);
    EXPECT_LE(abs(ts.compute_mean() - 0.5022), 0.0001);
    EXPECT_LE(abs(ts.compute_stddev() - 0.3072), 0.0001);
}

TEST(TimeSeriesStats, ComputeStd)
{
    std::vector<double> tc {0.15330792502278168, 0.9288688131475954, 0.6467927448900687, 0.8391664129724394, 0.04798762394687839,0.6393277709551051, 0.2727015569864173, 0.8360877258225698, 0.4975557719102256, 0.1608764499225922};
    TimeSeries ts(tc);
    EXPECT_LE(abs(ts.compute_stddev() - 0.3072), 0.0001);
}

TEST(TimeSeriesStats, NoArg)
{
    TimeSeries ts;
    ts.add_data(2.5);
    std::vector<double> values {4.5, 6.5};
    ts.extend(values);
    EXPECT_EQ(ts.compute_mean(), 4.5);
    EXPECT_LE(abs(ts.compute_stddev()-1.63299), 0.0001);
}
