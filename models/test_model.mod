stocks = ["WMT"];
stockfiles = ["models/WMT_data.dat"];
allocations = [10.4];
factors = ["10_year_treasury_maturity", "Real_Median_Household_Income", "Smoothed_US_Recession_Probabilities", "adjusted_unemployment_rate", "wti_spot_prices"];
factorfiles = ["models/10_year_treasury_maturity.dat", "models/Real_Median_Household_Income.dat", "models/Smoothed_US_Recession_Probabilities.dat", "models/adjusted_unemployment_rate.dat", "models/wti_spot_prices.dat"];
WMT = [-15.438835, 0.000000, -2.658913, 15.343503, -0.150512, 33.185866];
numtraces = 100;
numsteps = 1000;
