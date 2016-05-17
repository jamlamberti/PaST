#ifndef PAST_GBM_SIMULATION_H_
#define PAST_GBM_SIMULATION_H_
#include <iostream>
#include <vector>
#include <random>
#include "price_simulation.h"
#include "time_series.h"
#include <cilk/cilk.h>
#include <gtest/gtest.h>

class GBMSimulation : public PriceSimulation
{
    protected:
        double short_rate;

    public:
        GBMSimulation(double mu, double sigma, double starting, double csr) : PriceSimulation(mu, sigma, starting)
        {
            short_rate = csr; // constant short rate
        }

        void run_simulation();
        void run_simulation(unsigned int num_traces, unsigned int num_steps);
        void simulate_trace(unsigned int trace_id, unsigned int num_steps, double* prices, std::mt19937* generator);
};

#endif
