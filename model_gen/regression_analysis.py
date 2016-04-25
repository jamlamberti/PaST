import os
import csv
import datetime
from collections import defaultdict

import numpy as np

import scipy.stats
import scipy.linalg
from sklearn import linear_model
from sklearn.feature_selection import chi2
import yahoo_finance


def load_benchmark(benchmark_file):
    """Load a benchmark from a file"""
    benchmark_dict = {}
    with open(benchmark_file, 'r') as f_handle:
        csv_file = csv.reader(f_handle)

        for row in csv_file:
            benchmark_dict[datetime.datetime.strptime(
                row[0], '%Y-%m-%d').date()] = row[1]

    benchmark = []

    for row in sorted(benchmark_dict.items(), key=lambda x: x[0]):
        benchmark.append(row[1])

    for i, row in enumerate(
            sorted(benchmark_dict.items(), key=lambda x: x[0])):
        try:
            benchmark_dict[row[0]] = float(benchmark[i])
        except ValueError:
            try:
                benchmark[i] = (
                    float(benchmark[i - 1]) + float(benchmark[i + 1])) / 2
                benchmark_dict[row[0]] = benchmark[i]
            except ValueError:
                benchmark[i] = (
                    float(benchmark[i - 1]) + float(benchmark[i + 2])) / 2
                benchmark_dict[row[0]] = benchmark[i]
    return benchmark_dict


def wrap_benchmark(benchmark_file, dates):
    """Handle the different frequencies"""
    benchmark_data = {}
    b_d = load_benchmark(benchmark_file)
    bd_dates = sorted(b_d.keys())

    for d in dates:
        target = d - \
            min([d - di for di in bd_dates if (d - di) == abs(di - d)])
        benchmark_data[d] = b_d[target]

    return benchmark_data


def download_data(ticker, start_date, end_date):
    """Download data from YF"""
    share = yahoo_finance.Share(ticker)
    return share.get_historical(start_date, end_date)


def run_regression(ticker_pricing, benchmarks, dates):
    lr = linear_model.LinearRegression()
    benchmark_data = []
    for key in sorted(benchmarks.keys()):
        benchmark_data.append([benchmarks[key][di] for di in dates])

    benchmark_data = np.asmatrix(benchmark_data).T
    lr.fit(benchmark_data, ticker_pricing)
    
    print lr.coef_, lr.intercept_

def model_stock(ticker, start_date, end_date):
    """Model a stock"""
    dates = []
    data = []

    raw_data = download_data(ticker, start_date, end_date)

    for row in sorted(
            raw_data,
            key=lambda x: datetime.datetime.strptime(x['Date'], '%Y-%m-%d')):
        dates.append(
            datetime.datetime.strptime(row['Date'], '%Y-%m-%d').date())
        data.append(float(row['Adj_Close']))
    wti = wrap_benchmark(os.path.join('data', 'wti_spot_prices.csv'), dates)
    recession =  wrap_benchmark(os.path.join('data', 'Smoothed_US_Recession_Probabilities.csv'), dates)
    run_regression(data, {'wti': wti, 'recession': recession}, dates)
    #assert sorted(wti.keys()) == sorted(recession.keys())


if __name__ == '__main__':
    model_stock('WMT', '2014-03-01', '2016-03-01')
