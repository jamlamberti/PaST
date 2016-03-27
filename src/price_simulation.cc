#include "price_simulation.h"

PriceSimulation::PriceSimulation()
{
    mean = 0.0;
    std_dev = 1.0;
    starting_price = 100;
}

PriceSimulation::PriceSimulation(double mu, double sigma, double starting)
{
    mean = mu;
    std_dev = sigma;
    starting_price = starting;
}

PriceSimulation::~PriceSimulation()
{
}



void PriceSimulation::run_simulation()
{
    run_simulation(100, 500);
}

void PriceSimulation::run_simulation(unsigned int num_traces, unsigned int num_steps)
{
    std::cout << " [-] Not implemented!" << std::endl;
}

TEST(PriceSimulation, UnitTest)
{
    PriceSimulation ps(1.0, 1.5, 10);
    ps.run_simulation();
}
