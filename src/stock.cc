#include "stock.h"

Stock::Stock(std::string tick, std::string data_file)
{
    ticker = tick;
    load_data(data_file);
}

Stock::~Stock()
{
    delete(ts);
}

bool Stock::load_data(std::string data_file)
{
    std::ifstream file(data_file, std::ios_base::in);
    std::istream_iterator<double> start(file), end;
    std::vector<double> data(start, end);
    ts = new TimeSeries(data);
    return true;
}

TEST(Stock, StockLDTest)
{
    Stock s("XOM", "models/xom_data.dat");
}
