#include "gbm_weighted.h"


void GBMWeighted::run_simulation()
{
    //run_simulation(10, 500);
}

void GBMWeighted::run_simulation(unsigned int num_traces, std::vector<double>* model_prices)
{
    cilk_for (unsigned int i=0; i < num_traces; i++)
    {
        //GBMSimulation::simulate_trace(i, num_steps);
    }
}

void GBMWeighted::simulate_trace(unsigned int trace_id, std::vector<double>* model_prices, std::mt19937* generator)
{
    //std::cout << " [+] Running trace: " << trace_id << std::endl;
    unsigned int num_steps = model_prices->size();

    //std::mt19937 generator(trace_id*num_steps);
    std::normal_distribution<> normal(0, 1);

    //std::vector<double> prices;

    double prev = starting_price;
    double curr;
    double dt = 1.0; // 1.0/num_steps;
    double variance = pow(std_dev, 2);

    // prices.push_back(prev);

    for (unsigned int i = 1; i < num_steps; i++)
    {
        curr = prev*exp((short_rate - 0.5*variance)*dt + std_dev*sqrt(dt)*normal(*generator));
        curr = std::max(0.0, (gbm_weight*curr + (1-gbm_weight)*model_prices->at(i-1)));
        (*model_prices)[i-1] = curr;
        prev = curr;
    }
}

TEST(GBMWeighted, UnitTest)
{
}
