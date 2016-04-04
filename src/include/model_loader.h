#ifndef PAST_LINEAR_REGRESSION_H_
#define PAST_LINEAR_REGRESSION_H_

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <gtest/gtest.h>
#include <libconfig.h++>

class ModelLoader
{
    std::vector<std::string>* stocks;
    std::vector<std::string>* factors;
    std::vector< std::vector<double> >* factor_models;

    public:
        ModelLoader();
        ~ModelLoader();
        bool load_model(std::string);
};

#endif