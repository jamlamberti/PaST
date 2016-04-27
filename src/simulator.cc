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

void Simulator::simulate_benchmarks(unsigned int num_traces, unsigned int num_steps)
{
    double* final_prices = new double[num_traces];
    cilk_for(unsigned int iter = 0; iter < num_traces; iter++)
    {
        // simulate over each of the benchmark vector elements
        std::vector<double> prices;
        for (unsigned int i = 0; i < num_steps; i++)
        {
            prices.emplace_back(model->factor_models[0].back());
        }

        int cnt = 0;
        for (auto it = benchmarks.begin(); it != benchmarks.cend(); it++)
        {
            std::vector<double> benchmark = (*it)->simulate_trace(iter, num_steps);
            for (unsigned int i = 0; i < num_steps; i++)
            {
                prices[i] += model->factor_models[0][cnt]*benchmark[i];
            }
            cnt++;
        }
        final_prices[iter] = prices.back();
    }
    std::vector<double> vec(final_prices, final_prices+num_traces);
    delete(final_prices);
    TimeSeries ts(vec);
    RiskMeasures rm(ts);
    std::cout << ts.compute_mean() << " " << ts.compute_stddev() << " " << rm.value_at_risk(95, 10000) << " " << rm.value_at_risk(99, 10000) << std::endl;
}
