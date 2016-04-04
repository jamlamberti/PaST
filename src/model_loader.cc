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


    try
    {
        const libconfig::Setting &stocks_s = root["stocks"];
        int num_stocks = stocks_s.getLength();
        for (int i = 0; i < num_stocks; i++)
        {
            //const libconfig::Setting &stock = stocks_s[i];
            std::cout << stocks_s[i].c_str() << " ";
        }
        std::cout << std::endl;
    }
    catch (const libconfig::SettingNotFoundException &nfex)
    {
        std::cerr << "Stocks must be defined in the model" << std::endl;
        return false;
    }
    catch (const libconfig::SettingTypeException &stex)
    {
        std::cerr << "Something happened!! " << stex.what() << std::endl;
    }


    return true;
}

TEST(ModelLoader, ModelLoadParser)
{
    ModelLoader ml;
    bool rv = ml.load_model("/home/jlamberti/proj/PaST/models/sample.mod");
    EXPECT_EQ(rv, true);
}
