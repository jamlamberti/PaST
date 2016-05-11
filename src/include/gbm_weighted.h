#ifndef PAST_GBM_WEIGHTED_H_
#define PAST_GBM_WEIGHTED_H_
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "price_simulation.h"
#include "time_series.h"
#include <cilk/cilk.h>
#include <gtest/gtest.h>

class GBMWeighted : public PriceSimulation
{
    protected:
        double short_rate;
        double gbm_weight;
    
    public:
        GBMWeighted(double mu, double sigma, double starting, double csr, double gweight) : PriceSimulation(mu, sigma, starting)
        {
            short_rate = csr; // constant short rate
            gbm_weight = gweight; // How do we weight the model against truth
        }

        void run_simulation();
        void run_simulation(unsigned int num_traces, std::vector<double>* model_prices);
        void simulate_trace(unsigned int trace_id, std::vector<double>* model_prices);
};

#endif
