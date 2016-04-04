#include "sqrt_diffusion_simulation.h"


void SqrtDiffusionSimulation::run_simulation()
{
    run_simulation(10, 500);
}

void SqrtDiffusionSimulation::run_simulation(unsigned int num_traces, unsigned int num_steps)
{
    cilk_for (unsigned int i=0; i < num_traces; i++)
    {
        SqrtDiffusionSimulation::simulate_trace(i, num_steps);
    }
}

void SqrtDiffusionSimulation::simulate_trace(unsigned int trace_id, unsigned int num_steps)
{
    //std::cout << " [+] Running trace: " << trace_id << std::endl;
    std::mt19937 generator(trace_id*num_steps);
    std::normal_distribution<> normal(0, 1);

    std::vector<double> returns;
    
    double prev = starting_price;
    double curr;
    double dt = 1.0/num_steps;
    
    for (unsigned int i = 1; i < num_steps; i++)
    {
        curr = prev + kappa*(mean - fmax(prev, 0.0))*dt + std_dev * sqrt(dt*fmax(prev, 0.0))*normal(generator);
        returns.push_back(curr/fmax(prev, 0.0));
    }


    //TimeSeries ts(returns);
    //double ts_mean = ts.compute_mean();
    //double ts_std  = ts.compute_stddev();
    //std::cout << ts_mean << " " << ts_std << std::endl;
}

TEST(SqrtDiffusionSimulation, UnitTest)
{
    SqrtDiffusionSimulation ps(100, 0.1, 100, 0.05);
    ps.run_simulation();
}
