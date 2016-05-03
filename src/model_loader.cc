#include "model_loader.h"

ModelLoader::ModelLoader()
{
    num_traces = 0;
    num_steps = 0;
}

ModelLoader::~ModelLoader()
{
}

bool ModelLoader::load_model(std::string model_file)
{
    libconfig::Config cfg;
    try
    {
        cfg.readFile(model_file.c_str());
    }
    catch (const libconfig::FileIOException &fioex)
    {
        std::cerr << "Error when loading model file!" << std::endl;
        return false;
    }
    catch (const libconfig::ParseException &pex)
    {
        std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine();
        std::cerr << " - " << pex.getError() << std::endl;
        return false;
    }

    const libconfig::Setting &root = cfg.getRoot();

    int num_stocks, num_factors;
    try
    {
        const libconfig::Setting &stocks_s = root["stocks"];
        num_stocks = stocks_s.getLength();
        for (int i = 0; i < num_stocks; i++)
        {
            stocks.emplace_back(stocks_s[i].c_str());
        }
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "Stocks must be defined in the model" << std::endl;
        return false;
    }

    try
    {
        const libconfig::Setting &factors_s = root["factors"];
        num_factors = factors_s.getLength();
        for (int i = 0; i < num_factors; i++)
        {
            factors.emplace_back(factors_s[i].c_str());
        }
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "Factors must be defined in the model" << std::endl;
        return false;
    }

    try
    {
        for (auto it = stocks.cbegin(); it < stocks.cend(); it++)
        {
            const libconfig::Setting &factor = root[*it];
            int num_bench = factor.getLength();
            if (num_bench != num_factors + 1)
            {
                std::cerr << "Model must have # of factors + 1 elements in the factor model" << std::endl;
                return false;
            }

            std::vector<double> fm;
            for (int i = 0; i < num_bench; i++)
            {
                fm.emplace_back((double)factor[i]);
            }
            factor_models.emplace_back(fm);
        }
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "Factor Models must be defined for each stock in the model" << std::endl;
        return false;
    }
    
    try
    {
        const libconfig::Setting &stock_fi = root["stockfiles"];
        if (stock_fi.getLength() != num_stocks)
        {
            std::cerr << "Stocks must have the same size as stockfiles" << std::endl;
            return false;
        }
        
        for (int i = 0; i < num_stocks; i++)
        {
            stock_files.emplace_back(stock_fi[i].c_str());
        }
    } catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "Stockfiles must be defined in the model" << std::endl;
        return false;
    }

    try
    {

        const libconfig::Setting &stock_al = root["allocations"];
        if (stock_al.getLength() != num_stocks)
        {
            std::cerr << "Allocations must have the same size as stocks" << std::endl;
            return false;
        }

        for (int i = 0; i < num_stocks; i++)
        {
            stock_allocations.emplace_back((double)stock_al[i]);
        }
    } catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "Stock Allocations must be defined in the model" << std::endl;
        return false;
    }

    try
    {
        const libconfig::Setting &factor_fi = root["factorfiles"];
        if (factor_fi.getLength() != num_factors)
        {
            std::cerr << "Factor must have the same size as factorfiles" << std::endl;
            return false;
        }

        for (int i = 0; i < num_factors; i++)
        {
            factor_files.emplace_back(factor_fi[i].c_str());
        }
    } catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "Factorfiles must be defined in the model" << std::endl;
        return false;
    }

    try
    {
        num_traces = cfg.lookup("numtraces");
        num_steps = cfg.lookup("numsteps");
    } catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "num_traces and num_steps must be defined in the model" << std::endl;
        return false;
    }

    try
    {
        const char* num_threads = cfg.lookup("numthreads");
        __cilkrts_end_cilk();
        __cilkrts_set_param("nworkers", num_threads);
        //std::cout << " [+] Switching to " << num_threads << " threads" << std::endl;
    } catch (const libconfig::SettingNotFoundException &nfex)
    {
        // Just keep using cilk defaults!
    }

    std::cout << " [+] Successfully loaded " << num_stocks << " stocks and " << num_factors << " factors" << std::endl;

    return true;
}

TEST(ModelLoader, ModelLoadParser)
{
    ModelLoader ml;
    bool rv = ml.load_model("models/sample.mod");
    EXPECT_EQ(rv, true);
}
