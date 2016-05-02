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
    port_worth = 0.0;
    for (auto it = model->stocks.begin(); it != model->stocks.cend(); it++)
    {
        Stock s = Stock(*it, model->stock_files.at(cnt));
        port_worth += model->stock_allocations.at(cnt)*s.ts->values.at(0);
        cnt++;
    }

    cnt = 0;

    for (auto it = model->factors.begin(); it != model->factors.cend(); it++)
    {
        Stock s = Stock(*it, model->factor_files.at(cnt));
        double mean = s.ts->compute_mean();
        double stddev = s.ts->compute_stddev(1);
        double sprice = s.ts->values.back();
        GBMSimulation* gbms = new GBMSimulation(mean, stddev, sprice, 0.05);
        benchmarks.push_back(gbms);
        cnt++;
    }
}

void Simulator::simulate_benchmarks()
{
    return Simulator::simulate_benchmarks(model->num_traces, model->num_steps);
}

void Simulator::simulate_benchmarks(unsigned int num_traces, unsigned int num_steps)
{
    double* final_prices = new double[num_traces];
    cilk_for(unsigned int iter = 0; iter < num_traces; iter++)
    {
        //std::vector<double>portfolio_prices;
        // simulate over each of the benchmark vector elements
        std::vector<std::vector<double>> prices;
        for (unsigned int j = 0; j < model->stocks.size(); j++)
        {
            std::vector<double> curr;
            for (unsigned int i = 0; i < num_steps; i++)
            {
                curr.emplace_back(model->factor_models[j].back());
            }
            prices.push_back(curr);
        }

        int cnt = 0;

        for (auto it = benchmarks.begin(); it != benchmarks.cend(); it++)
        {
            std::vector<double> benchmark = (*it)->simulate_trace(iter, num_steps);
            for (unsigned int j = 0; j < prices.size(); j++)
            {
                for (unsigned int i = 0; i < num_steps; i++)
                {
                    prices[j][i] += model->factor_models[j][cnt]*benchmark[i];
                }
            }
            cnt++;
        }
        double final_price = 0.0;
        cnt = 0;
        for (unsigned int i = 0; i < prices.size(); i++)
        {
            // Should use values from file
            final_price += model->stock_allocations[i] * prices[i].back();
        }
        final_prices[iter] = final_price;
    }
    std::vector<double> vec(final_prices, final_prices+num_traces);
    delete(final_prices);
    TimeSeries ts(vec);
    RiskMeasures rm(ts);
    std::cout << ts.compute_mean() << " " << ts.compute_stddev() << " " << rm.value_at_risk(95, port_worth) << " " << rm.value_at_risk(99, port_worth) << std::endl;
}
