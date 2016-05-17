#ifndef PAST_SIMULATOR_H_
#define PAST_SIMULATOR_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <cilk/reducer_max.h>
#include <gtest/gtest.h>
#include "model_loader.h"
#include "stock.h"
#include "gbm_simulation.h"
#include "gbm_weighted.h"
#include "sqrt_diffusion_simulation.h"
#include "jump_diffusion_simulation.h"
#include "sqrt_diffusion_weighted.h"
#include "jump_diffusion_weighted.h"
#include "risk_measures.h"

class Simulator
{
    ModelLoader* model;
    std::vector<PriceSimulation*> benchmarks;
    double port_worth;
    std::vector<PriceSimulation*> weighted_sims;
    
    public:
        Simulator(ModelLoader*);
        ~Simulator();
        void model_benchmarks();
        void simulate_benchmarks();
        void simulate_benchmarks(unsigned int, unsigned int);
};

#endif
