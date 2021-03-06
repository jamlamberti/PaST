#include "sqrt_diffusion_simulation.h"


void SqrtDiffusionSimulation::run_simulation()
{
    run_simulation(10, 500);
}

void SqrtDiffusionSimulation::run_simulation(unsigned int num_traces, unsigned int num_steps)
{
    cilk_for (unsigned int i=0; i < num_traces; i++)
    {
        // SqrtDiffusionSimulation::simulate_trace(i, num_steps);
    }
}

void SqrtDiffusionSimulation::simulate_trace(unsigned int trace_id, unsigned int num_steps, double* prices, std::mt19937* generator)
{
    //std::cout << " [+] Running trace: " << trace_id << std::endl;
    //std::mt19937 generator(trace_id*num_steps);
    std::normal_distribution<> normal(0, 1);

    //std::vector<double> prices;

    double prev = starting_price;
    double curr;
    double dt = 1.0/num_steps;
    
    //prices.push_back(prev);
    prices[0] = prev;

    for (unsigned int i = 1; i < num_steps; i++)
    {
        curr = prev + kappa*(long_term_mean - fmax(prev, 0.0))*dt + std_dev * sqrt(dt*fmax(prev, 0.0))*normal(*generator);
        // returns.push_back(curr/fmax(prev, 0.0));
        prices[i] = fmax(curr, 0.0);
        prev = curr;
    }

    //return prices;
}

TEST(SqrtDiffusionSimulation, UnitTest)
{
    SqrtDiffusionSimulation ps(100, 0.1, 100, 0.05, 105);
    ps.run_simulation();
}
