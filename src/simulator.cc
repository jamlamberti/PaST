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
    for (auto it = weighted_sims.begin(); it != weighted_sims.cend(); it++)
    {
        delete(*it);
    }
}

void Simulator::model_benchmarks()
{
    benchmarks.clear();
    weighted_sims.clear();
    // push onto benchmarks vector
    int cnt = 0;
    port_worth = 0.0;
    // starting_prices
    for (auto it = model->stocks.begin(); it != model->stocks.cend(); it++)
    {
        Stock s = Stock(*it, model->stock_files.at(cnt));
        port_worth += model->stock_allocations.at(cnt)*s.ts->values.back();
        double mean = s.ts->compute_mean_parallel();
        double vol = s.ts->compute_volatility();
        double starting = s.ts->values.back();
        switch(model->smodel_params[cnt].size())
        {
            case 1:
            {
                GBMWeighted* gbmw = new GBMWeighted(mean, vol, starting, model->short_rate, model->smodel_params[cnt][0]);
                weighted_sims.push_back(gbmw);
            }

            case 3:
            {
                SqrtDiffusionWeighted* sqrtw = new SqrtDiffusionWeighted(mean, vol, starting, model->smodel_params[cnt][0], model->smodel_params[cnt][1], model->smodel_params[cnt][2]);
                weighted_sims.push_back(sqrtw);
            }

            case 4:
            {

                JumpDiffusionWeighted* jumpw = new JumpDiffusionWeighted(mean, vol, starting, model->smodel_params[cnt][0], model->smodel_params[cnt][1], model->smodel_params[cnt][2], model->smodel_params[cnt][3]);
                weighted_sims.push_back(jumpw);
            }
        }
        cnt++;
    }

    std::cout << "Current Worth: " << port_worth << std::endl;

    cnt = 0;

    for (auto it = model->factors.begin(); it != model->factors.cend(); it++)
    {
        Stock s = Stock(*it, model->factor_files.at(cnt));
        double mean = s.ts->compute_mean_parallel();
        double stddev = s.ts->compute_volatility();
        double sprice = s.ts->values.back();
        unsigned int num_params = model->fmodel_params[cnt].size();
        switch (num_params)
        {

            case 0:
            {
                //std::cout << " [+] Using GBM" << std::endl;
                GBMSimulation* gbms = new GBMSimulation(mean, stddev, sprice, model->short_rate);
                benchmarks.push_back(gbms);
                break;
            }

            case 2:
            {
                //std::cout << " [+] Using SQRT Diffusion" << std::endl;
                SqrtDiffusionSimulation* sqrts = new SqrtDiffusionSimulation(mean, stddev, sprice, model->fmodel_params[cnt][0], model->fmodel_params[cnt][1]);
                benchmarks.push_back(sqrts);
                break;
            }

            case 3:
            {
                JumpDiffusionSimulation* jmps = new JumpDiffusionSimulation(mean, stddev, sprice, model->fmodel_params[cnt][0], model->fmodel_params[cnt][1], model->fmodel_params[cnt][2]);
                benchmarks.push_back(jmps);
                break;
            }

            default:
            {
                std::cout << " [-] Unknown model parameters, reverting to Geometric Brownian Motion model" << std::endl;
                GBMSimulation* gbms = new GBMSimulation(mean, stddev, sprice, model->short_rate);
                benchmarks.push_back(gbms);
                
                break;
            }
        }

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
    unsigned int chunk_size = 8;
    cilk_for(unsigned int chunk_iters = 0; chunk_iters < num_traces; chunk_iters+=chunk_size)
    {
        std::vector<std::vector<double>> prices;
        std::vector<double> curr;
        for (unsigned int j = 0; j < model->stocks.size(); j++)
        {
            curr.clear();
            for (unsigned int i = 0; i < num_steps; i++)
            {
                curr.emplace_back(model->factor_models[j].back());
            }
            prices.push_back(curr);
        }
        double* benchmark = new double[num_steps];
        std::mt19937 generator(chunk_iters);
        for (unsigned int iter_cnt = 0; iter_cnt < chunk_size; iter_cnt++)
        {
            unsigned int iter = iter_cnt + chunk_iters;
            if (iter >= num_traces) break;
            if (!iter_cnt)
                generator.seed(iter);
            // std::vector<double>portfolio_prices;
            // simulate over each of the benchmark vector elements

            int cnt = 0;
            for (auto it = benchmarks.begin(); it != benchmarks.cend(); it++)
            {
                (*it)->simulate_trace(iter*(cnt+2) + cnt, num_steps, benchmark, &generator);
                for (unsigned int j = 0; j < prices.size(); j++)
                {
                    for (unsigned int i = 0; i < num_steps; i++)
                    {
                        prices[j][i] += model->factor_models[j][cnt]*benchmark[i];
                    }
                }
                cnt++;
            }
            
            for (unsigned int j = 0; j < prices.size(); j++)
            {
                weighted_sims[j]->simulate_trace(iter*(cnt+1), &(prices[j]), &generator);
                // weighted_sims[j] = updated;
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
            }
            final_prices[iter] = curr_price;
            drawdowns[iter] = mdd;
        }
        delete[] benchmark;
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
    std::cout << "Mean:    " << ts.compute_mean_parallel() << std::endl;
    std::cout << "Stddev:  " << ts.compute_stddev() << std::endl;
    std::cout << "95% VaR: " << rm.value_at_risk(95, port_worth) << std::endl;
    std::cout << "99% VaR: " << rm.value_at_risk(99, port_worth) << std::endl;
    std::cout << "Max DD:  " << max_drawdown.get_value() << "%" << std::endl;
    std::cout << "Avg MDD: " << drawdown_sum.get_value()/num_traces << "%" << std::endl;
}
