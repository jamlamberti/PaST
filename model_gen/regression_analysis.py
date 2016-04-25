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


def wrap_benchmark(benchmark_file, freq, dates):
    #benchmark_data = {d: None for d in dates}
    pass


def download_data(ticker, start_date, end_date):
    """Download data from YF"""
    share = yahoo_finance.Share(ticker)
    return share.get_historical(start_date, end_date)


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
