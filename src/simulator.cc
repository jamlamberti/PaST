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
        Stock s = Stock(*it, model->factor_files.at(cnt++));
        double mean = s.ts->compute_mean();
        double stddev = s.ts->compute_stddev(1);
        double sprice = s.ts->values.back();
        GBMSimulation* gbms = new GBMSimulation(mean, stddev, sprice, 0.05);
        benchmarks.push_back(gbms);
    }
}


void Simulator::simulate_benchmarks()
{
    // simulate over each of the benchmark vector elements
    std::vector<double> prices;
    for (int i = 0; i < 365; i++)
    {
        prices.emplace_back(model->factor_models[0].back());
    }

    int cnt = 0;
    for (auto it = benchmarks.begin(); it != benchmarks.cend(); it++)
    {
        std::vector<double> benchmark = (*it)->simulate_trace(0, 365);
        for (int i = 0; i < 365; i++)
        {
            prices[i] += model->factor_models[0][cnt]*benchmark[i];
        }
        cnt++;
    }
    for (auto it = prices.begin(); it != prices.cend(); it++)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
}
