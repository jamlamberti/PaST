#ifndef PAST_MODEL_LOADER_H_
#define PAST_MODEL_LOADER_H_

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <gtest/gtest.h>
#include <libconfig.h++>

class ModelLoader
{

    public:
        std::vector<std::string> stocks;
        std::vector<std::string> stock_files;
        std::vector<std::string> factors;
        std::vector<std::string> factor_files;
        std::vector< std::vector<double>> factor_models;
        ModelLoader();
        ~ModelLoader();
        bool load_model(std::string);
};

#endif
