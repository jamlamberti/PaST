#include "gbm_simulation.h"


void GBMSimulation::run_simulation()
{
    run_simulation(10, 500);
}

void GBMSimulation::run_simulation(unsigned int num_traces, unsigned int num_steps)
{
    cilk_for (unsigned int i=0; i < num_traces; i++)
    {
        //GBMSimulation::simulate_trace(i, num_steps);
    }
}

void GBMSimulation::simulate_trace(unsigned int trace_id, unsigned int num_steps, double* prices, std::mt19937* generator)
{
    //std::cout << " [+] Running trace: " << trace_id << std::endl;
    //std::mt19937 generator(trace_id*num_steps);
    std::normal_distribution<> normal(0, 1);

    //std::vector<double> returns;
    //std::vector<double> prices;

    double prev = starting_price;
    double curr;
    double dt = 1.0; // 1.0/num_steps;
    double variance = pow(std_dev, 2);
    //prices.push_back(prev);
    prices[0] = prev;
    for (unsigned int i = 1; i < num_steps; i++)
    {
        curr = prev*exp((short_rate - 0.5*variance)*dt + std_dev*sqrt(dt)*normal(*generator));
        //returns.push_back(curr/prev);
        //prices.push_back(curr);
        prices[i] = curr;
        prev = curr;
        
    }

    //TimeSeries ts(returns);
    // double ts_mean = ts.compute_mean();
    // double ts_std  = ts.compute_stddev();
    //std::cout << ts_mean << " " << ts_std << std::endl;
    //return prices;
}

TEST(GBMSimulation, UnitTest)
{
    GBMSimulation ps(100, 0.1, 100, 0.05);
    ps.run_simulation();
}
