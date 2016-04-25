#ifndef PAST_SIMULATOR_H_
#define PAST_SIMULATOR_H_

#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <gtest/gtest.h>
#include "model_loader.h"

class Simulator
{
    ModelLoader model;
    public:
        Simulator(ModelLoader);
        ~Simulator();
};

#endif
