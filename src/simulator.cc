#include "simulator.h"

Simulator::Simulator(ModelLoader* ml)
{
    // Take in the model and act as a driver
    model = ml;
}

Simulator::~Simulator()
{
    for (auto it = benchmarks.begin(); it != benchmarks.cend(); it++)
    {
        delete(*it);
    }
}


void Simulator::model_benchmarks()
{
    benchmarks.clear();
    // push onto benchmarks vector
    int cnt = 0;
    for (auto it = model->factors.begin(); it != model->factors.cend(); it++)
    {
        Stock* s = new Stock(*it, model->factor_files.at(cnt++));
        benchmarks.push_back(s);
    }
}


void Simulator::simulate_benchmarks()
{
    // simulate over each of the benchmark vector elements
}
