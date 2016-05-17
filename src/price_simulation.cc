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
    unsigned int num_traces = 100;
    unsigned int num_steps = 500;
    std::cout << " [+] Using defaults of: (" << num_traces << ", " << num_steps << ")"  << std::endl;
    run_simulation(num_traces, num_steps);
}

void PriceSimulation::run_simulation(unsigned int num_traces, unsigned int num_steps)
{
    std::cout << " [-] Not implemented!" << std::endl;
}

void PriceSimulation::simulate_trace(unsigned int num_traces, unsigned int num_steps, double* vec, std::mt19937* generator)
{
    std::cout << " [-] Not implemented!" << std::endl;
}

void PriceSimulation::simulate_trace(unsigned int num_traces, std::vector<double>* model_prices, std::mt19937* generator)
{
    std::cout << " [-] Not implemented!" << std::endl;
}

TEST(PriceSimulation, UnitTest)
{
    PriceSimulation ps(1.0, 1.5, 10);
    ps.run_simulation();
}

TEST(PriceSimulation, NoArg)
{
    PriceSimulation ps;
    ps.run_simulation(10, 5);
}
