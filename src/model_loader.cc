#include "model_loader.h"

ModelLoader::ModelLoader()
{
    stocks = new std::vector<std::string>();
    factors = new std::vector<std::string>();
    factor_models = new std::vector< std::vector<double> >();
}

ModelLoader::~ModelLoader()
{
    delete(stocks);
    delete(factors);
    delete(factor_models);
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
            stocks->emplace_back(stocks_s[i].c_str());
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
            factors->emplace_back(factors_s[i].c_str());
        }
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "Factors must be defined in the model" << std::endl;
        return false;
    }

    try
    {
        for (auto it = stocks->cbegin(); it < stocks->cend(); it++)
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
            factor_models->emplace_back(fm);
        }
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "Factor Models must be defined for each stock in the model" << std::endl;
        return false;
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
