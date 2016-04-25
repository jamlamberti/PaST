#include "simulator.h"

Simulator::Simulator(ModelLoader* ml)
{
    // Take in the model and act as a driver
    model = ml;
}

Simulator::~Simulator()
{
    // Do nothing for now
}


void Simulator::model_benchmarks()
{
    benchmarks.clear();
    // push onto benchmarks vector
    //for (auto it = ml.factors->begin(); it != ml.factors->cend(); it++)
    //{
    //    Stock s = new Stock(*it,  
    //    benchmarks.push_back(
    //}
}


void Simulator::simulate_benchmarks()
{
    // simulate over each of the benchmark vector elements    
}
