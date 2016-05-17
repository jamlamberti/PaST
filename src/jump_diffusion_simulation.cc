#include "jump_diffusion_simulation.h"


void JumpDiffusionSimulation::run_simulation()
{
    run_simulation(10, 500);
}

void JumpDiffusionSimulation::run_simulation(unsigned int num_traces, unsigned int num_steps)
{
    cilk_for (unsigned int i=0; i < num_traces; i++)
    {
        //JumpDiffusionSimulation::simulate_trace(i, num_steps);
    }
}

void JumpDiffusionSimulation::simulate_trace(unsigned int trace_id, unsigned int num_steps, double* prices, std::mt19937* generator)
{
    //std::mt19937 generator(trace_id*num_steps);
    std::normal_distribution<> normal(0, 1);
    //std::vector<double> prices;

    double prev = starting_price;
    double curr;
    double dt = 1.0; // 1.0/num_steps;
    double variance = pow(std_dev, 2);
    double jump_correction = poisson_intensity * (exp(mean + 0.5 * pow(delta, 2)) - 1);
    std::poisson_distribution<> poisson(poisson_intensity*dt);
    //prices.push_back(prev);
    prices[0] = prev;

    for (unsigned int i = 1; i < num_steps; i++)
    {
        //curr = fmax(prev + riskless_sr*(delta - prev)*dt + std_dev * sqrt(dt*prev)*normal(generator), 0.0);
        curr = fmax(prev*exp(dt*(riskless_sr - jump_correction - 0.5*variance) + std_dev*sqrt(dt)*normal(*generator)) + (exp(mean + delta + normal(*generator))-1)*poisson(*generator), 0.0);
        prices[i] = curr;
        prev = curr;
    }

    //return prices;
}

TEST(JumpDiffusionSimulation, UnitTest)
{
    JumpDiffusionSimulation ps(100, 0.1, 100, 0.005, -0.1, 0.01);
    ps.run_simulation();
}
