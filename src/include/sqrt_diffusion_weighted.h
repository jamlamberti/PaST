#ifndef PAST_SQRT_DIFFUSION_WEIGHTED_H_
#define PAST_SQRT_DIFFUSION_WEIGHTED_H_
#include <iostream>
#include <vector>
#include <random>
#include "price_simulation.h"
#include "time_series.h"
#include <cilk/cilk.h>
#include <gtest/gtest.h>

class SqrtDiffusionWeighted : public PriceSimulation
{
    protected:
        double kappa;
        double long_term_mean;
        double model_weight;
        //void simulate_trace(unsigned int trace_id, unsigned int num_steps);

    public:
        SqrtDiffusionWeighted(double mu, double sigma, double starting, double weight, double k, double ltm) : PriceSimulation(mu, sigma, starting)
        {
            kappa = k;
            long_term_mean = ltm;
            model_weight = weight;
        }

        void run_simulation();
        void run_simulation(unsigned int num_traces, unsigned int num_steps);
        void simulate_trace(unsigned int, std::vector<double>*, std::mt19937*);
};

#endif
