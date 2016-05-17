#include "sqrt_diffusion_weighted.h"


void SqrtDiffusionWeighted::run_simulation()
{
    //run_simulation(10, 500);
}

void SqrtDiffusionWeighted::run_simulation(unsigned int num_traces, unsigned int num_steps)
{
    cilk_for (unsigned int i=0; i < num_traces; i++)
    {
        //SqrtDiffusionWeighted::simulate_trace(i, num_steps);
    }
}

void SqrtDiffusionWeighted::simulate_trace(unsigned int trace_id, std::vector<double>* model_prices, std::mt19937* generator)
{
    unsigned int num_steps = model_prices->size();
    //std::mt19937 generator(trace_id*num_steps);
    std::normal_distribution<> normal(0, 1);


    double prev = starting_price;
    double curr;
    double dt = 1.0; // 1.0/num_steps;
    
    // prices.push_back(prev);

    for (unsigned int i = 1; i < num_steps; i++)
    {
        curr = prev + kappa*(long_term_mean - fmax(prev, 0.0))*dt + std_dev * sqrt(dt*fmax(prev, 0.0))*normal(*generator);
        curr = (curr*model_weight + (1-model_weight)*model_prices->at(i));
        model_prices->at(i) = fmax(curr, 0.0);
        prev = curr;
    }

}

TEST(SqrtDiffusionWeighted, UnitTest)
{
}
