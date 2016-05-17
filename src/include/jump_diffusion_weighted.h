#ifndef PAST_JUMP_DIFFUSION_WEIGHTED_H_
#define PAST_JUMP_DIFFUSION_WEIGHTED_H_
#include <iostream>
#include <vector>
#include <random>
#include "price_simulation.h"
#include "time_series.h"
#include <cilk/cilk.h>
#include <gtest/gtest.h>

class JumpDiffusionWeighted : public PriceSimulation
{
    protected:
        double model_weight;
        double riskless_sr;
        double delta;
        double poisson_intensity;
    public:
        JumpDiffusionWeighted(double mu, double sigma, double starting, double weight, double rsr, double delt, double poissoni) : PriceSimulation(mu, sigma, starting)
        {
            model_weight = weight;
            riskless_sr = rsr;
            delta = delt;
            poisson_intensity = poissoni;
        }

        void run_simulation();
        void run_simulation(unsigned int num_traces, unsigned int num_steps);
        void simulate_trace(unsigned int, std::vector<double>*, std::mt19937*);
};

#endif
