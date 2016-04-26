#ifndef PAST_SIMULATOR_H_
#define PAST_SIMULATOR_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <gtest/gtest.h>
#include "model_loader.h"
#include "stock.h"
#include "gbm_simulation.h"

class Simulator
{
    ModelLoader* model;
    std::vector<GBMSimulation*> benchmarks;
    public:
        Simulator(ModelLoader*);
        ~Simulator();
        void model_benchmarks();
        void simulate_benchmarks();
};

#endif
