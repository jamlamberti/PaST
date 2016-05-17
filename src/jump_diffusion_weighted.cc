#include "jump_diffusion_weighted.h"


void JumpDiffusionWeighted::run_simulation()
{
    //run_simulation(10, 500);
}

void JumpDiffusionWeighted::run_simulation(unsigned int num_traces, unsigned int num_steps)
{
    cilk_for (unsigned int i=0; i < num_traces; i++)
    {
        //JumpDiffusionSimulation::simulate_trace(i, num_steps);
    }
}

void JumpDiffusionWeighted::simulate_trace(unsigned int trace_id, std::vector<double>* model_prices, std::mt19937* generator)
{
    unsigned int num_steps = model_prices->size();
    //std::mt19937 generator(trace_id*num_steps);
    std::normal_distribution<> normal(0, 1);

    double prev = starting_price;
    double curr;
    double dt = 1.0; // 1.0/num_steps;
    double variance = pow(std_dev, 2);
    double jump_correction = poisson_intensity * (exp(mean + 0.5 * pow(delta, 2)) - 1);
    std::poisson_distribution<> poisson(poisson_intensity*dt);

    for (unsigned int i = 1; i < num_steps; i++)
    {
        curr = fmax(prev*exp(dt*(riskless_sr - jump_correction - 0.5*variance) + std_dev*sqrt(dt)*normal(*generator)) + (exp(mean + delta + normal(*generator))-1)*poisson(*generator), 0.0);
        curr = (curr*model_weight) + (1-model_weight)*model_prices->at(i);
        model_prices->at(i) = curr;
        prev = curr;
    }

}

TEST(JumpDiffusionWeighted, UnitTest)
{
}
