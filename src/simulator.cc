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
    std::cout << "Current Worth: " << port_worth << std::endl;

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
    std::cout << " [+] Running with " << __cilkrts_get_nworkers() << " threads" << std::endl;
    double* final_prices = new double[num_traces];
    double* drawdowns = new double[num_traces];
    
    cilk_for(unsigned int iter = 0; iter < num_traces; iter++)
    {
        // std::vector<double>portfolio_prices;
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
        cnt = 0;
        double mdd = 0.0;
        double drawdown = 0.0;
        double peak = 0.0;
        double curr_price = 0.0;
        for (unsigned int j = 0; j < num_steps; j++)
        {
            curr_price = 0.0;

            for (unsigned int i = 0; i < prices.size(); i++)
            {
                // Should use values from file
                curr_price += std::max(0.0, model->stock_allocations[i] * prices[i][j]);
            }
            if (curr_price > peak)
            {
                peak = curr_price;
            }

            drawdown = 100*(peak-curr_price)/peak;

            if (drawdown > mdd)
            {
                mdd = drawdown;
            }
            //portfolio_prices.emplace_back(curr_price);
        }
        final_prices[iter] = curr_price; // portfolio_prices.back();
        //TimeSeries ts(portfolio_prices);
        //RiskMeasures rm(ts);
        drawdowns[iter] = mdd;
    }
    std::vector<double> vec(final_prices, final_prices+num_traces);
    delete[] final_prices;
    // Find the max drawdown

    cilk::reducer< cilk::op_max<double> > max_drawdown;
    cilk::reducer< cilk::op_add<double> > drawdown_sum(0);
    cilk_for(unsigned int iter = 0; iter < num_traces; iter++)
    {
        max_drawdown->calc_max(drawdowns[iter]);
        *drawdown_sum += drawdowns[iter];
    }

    delete[] drawdowns;
    TimeSeries ts(vec);
    RiskMeasures rm(ts);
    std::cout << "Mean:    " << ts.compute_mean() << std::endl;
    std::cout << "Stddev:  " << ts.compute_stddev() << std::endl;
    std::cout << "95% VaR: " << rm.value_at_risk(95, port_worth) << std::endl;
    std::cout << "99% VaR: " << rm.value_at_risk(99, port_worth) << std::endl;
    std::cout << "Max DD:  " << max_drawdown.get_value() << "%" << std::endl;
    std::cout << "Avg MDD: " << drawdown_sum.get_value()/num_traces << "%" << std::endl;
}
