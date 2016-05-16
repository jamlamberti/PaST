#ifndef PAST_PRICE_SIMULATION_H_
#define PAST_PRICE_SIMULATION_H_
#include <iostream>
#include <vector>
#include <gtest/gtest.h>
class PriceSimulation
{
    protected:
        double mean;
        double std_dev;
        double starting_price;

    public:
        PriceSimulation();
        PriceSimulation(double mu, double sigma, double starting);
        virtual ~PriceSimulation();
        void run_simulation();
        void run_simulation(unsigned int num_traces, unsigned int num_steps);
        virtual std::vector<double> simulate_trace(unsigned int, unsigned int);
        virtual void simulate_trace(unsigned int, std::vector<double>*);
};
#endif
