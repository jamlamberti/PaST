#ifndef PAST_SIMULATOR_H_
#define PAST_SIMULATOR_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <cilk/cilk.h>
#include <cilk/reducer_max.h>
#include <gtest/gtest.h>
#include "model_loader.h"
#include "stock.h"
#include "gbm_simulation.h"
#include "risk_measures.h"

class Simulator
{
    ModelLoader* model;
    std::vector<GBMSimulation*> benchmarks;
    double port_worth;
    public:
        Simulator(ModelLoader*);
        ~Simulator();
        void model_benchmarks();
        void simulate_benchmarks();
        void simulate_benchmarks(unsigned int, unsigned int);
};

#endif
