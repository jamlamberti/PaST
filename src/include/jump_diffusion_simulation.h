#ifndef PAST_JUMP_DIFFUSION_SIMULATION_H_
#define PAST_JUMP_DIFFUSION_SIMULATION_H_
#include <iostream>
#include <vector>
#include <random>
#include "price_simulation.h"
#include "time_series.h"
#include <cilk/cilk.h>
#include <gtest/gtest.h>

class JumpDiffusionSimulation : public PriceSimulation
{
    protected:
        double riskless_sr;
        double delta;
        double poisson_intensity;
    public:
        JumpDiffusionSimulation(double mu, double sigma, double starting, double rsr, double delt, double poissoni) : PriceSimulation(mu, sigma, starting)
        {
            riskless_sr = rsr;
            delta = delt;
            poisson_intensity = poissoni;
        }

        void run_simulation();
        void run_simulation(unsigned int num_traces, unsigned int num_steps);
        void simulate_trace(unsigned int, unsigned int, double* prices);
};

#endif
