#include "risk_measures.h"

RiskMeasures::RiskMeasures(TimeSeries t)
{
    ts = t;
}

RiskMeasures::~RiskMeasures()
{ }

double RiskMeasures::value_at_risk(int confidence, double worth)
{
    if ((confidence < 0) || confidence > 100)
    {
        std::cerr << "confidence must be in the range of [0, 100]" << std::endl;
        return -1;
    }

    std::vector<double> returns = ts.compute_returns();
    std::sort(returns.begin(), returns.end());
    double inc = 100.0/returns.size();
    double val = 0;
    auto it = returns.begin();
    while ((val < (100-confidence)) && (it < returns.cend()))
    {
        val += inc;
        it++;
    }
    it--;
    return -1*(*it)*worth;
}

double RiskMeasures::max_drawdown()
{
    double mdd = 0.0;
    double drawdown = 0.0;
    double peak = ts.values.at(0);

    for (auto it = ts.values.begin(); it != ts.values.cend(); it++)
    {
        if (*it > peak)
        {
            peak = *it;
        }
        drawdown = 100.0*(peak-*it)/peak;

        if (drawdown > mdd)
        {
            mdd = drawdown;
        }
    }
    return mdd;
}

TEST(RiskMeasures, RiskMeasuresVaR)
{
    std::vector<double> prices = {100.0, 98.0, 97.02, 98.96, 102.92};
    TimeSeries ts(prices);
    RiskMeasures rm(prices);
    double var = rm.value_at_risk(75, 100.0);
    EXPECT_LT(abs(var-2.0), 1e-8);
}
