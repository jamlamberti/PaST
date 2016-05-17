#ifndef PAST_MODEL_LOADER_H_
#define PAST_MODEL_LOADER_H_

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <gtest/gtest.h>
#include <libconfig.h++>

class ModelLoader
{

    public:
        std::vector<std::string> stocks;
        std::vector<std::string> stock_files;
        std::vector<double> stock_allocations;
        std::vector<std::string> factors;
        std::vector<std::string> factor_files;
        std::vector< std::vector<double> > factor_models;
        std::vector< std::vector<double> > fmodel_params;
        std::vector< std::vector<double> > smodel_params;

        // Global Params
        double short_rate;
        
        unsigned int num_traces;
        unsigned int num_steps;
        ModelLoader();
        ~ModelLoader();
        bool load_model(std::string);
};

#endif
