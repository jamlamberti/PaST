#ifndef PAST_SQRT_DIFFUSION_SIMULATION_H_
#define PAST_SQRT_DIFFUSION_SIMULATION_H_
#include <iostream>
#include <vector>
#include <random>
#include "price_simulation.h"
#include "time_series.h"
#include <cilk/cilk.h>
#include <gtest/gtest.h>

class SqrtDiffusionSimulation : public PriceSimulation
{
    protected:
        double kappa;
        double long_term_mean;
        //void simulate_trace(unsigned int trace_id, unsigned int num_steps);

    public:
        SqrtDiffusionSimulation(double mu, double sigma, double starting, double k, double ltm) : PriceSimulation(mu, sigma, starting)
        {
            kappa = k;
            long_term_mean = ltm;
        }

        void run_simulation();
        void run_simulation(unsigned int num_traces, unsigned int num_steps);
        std::vector<double> simulate_trace(unsigned int, unsigned int);
};

#endif
