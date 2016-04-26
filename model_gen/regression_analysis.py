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
        # csv_file = csv.reader(f_handle)
        csv_file = csv.DictReader(f_handle)

        for row in csv_file:
            benchmark_dict[datetime.datetime.strptime(
                row['DATE'], '%Y-%m-%d').date()] = row['VALUE']

    benchmark = []

    for row in sorted(benchmark_dict.items(), key=lambda x: x[0]):
        benchmark.append(row[1])

    for i, row in enumerate(
            sorted(benchmark_dict.items(), key=lambda x: x[0])):
        try:
            benchmark_dict[row[0]] = float(benchmark[i])
        except ValueError:
            cnt = 1
            while True:
                try:
                    benchmark[i] = (
                        float(benchmark[i - 1]) + float(benchmark[i + cnt])) / 2
                    benchmark_dict[row[0]] = benchmark[i]
                except ValueError:
                    cnt += 1
                else:
                    break
    return benchmark_dict


def wrap_benchmark(benchmark_file, dates):
    """Handle the different frequencies"""
    benchmark_data = {}
    b_d = load_benchmark(benchmark_file)
    bd_dates = sorted(b_d.keys())

    for date in dates:
        target = date - \
            min([date - di for di in bd_dates if (
                date - di) == abs(di - date)])
        benchmark_data[date] = b_d[target]

    return benchmark_data


def download_data(ticker, start_date, end_date):
    """Download data from YF"""
    share = yahoo_finance.Share(ticker)
    return share.get_historical(start_date, end_date)


def run_regression(ticker_pricing, benchmarks, dates):
    """Run a Regression Model"""
    clr = linear_model.LinearRegression()
    benchmark_data = []
    for key in sorted(benchmarks.keys()):
        benchmark_data.append([benchmarks[key][di] for di in dates])

    benchmark_data = np.asmatrix(benchmark_data).T
    clr.fit(benchmark_data, ticker_pricing)

    return clr.coef_, clr.intercept_


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
    benchmarks = {}
    for root, _, files in os.walk(os.path.abspath('data')):
        for data_file in files:
            try:
                benchmarks[os.path.splitext(os.path.basename(data_file))[0]] = wrap_benchmark(
                    os.path.join(root, data_file),
                    dates)
            except Exception, e:
                print e
                print data_file
    coef, intercept = run_regression(data, benchmarks, dates)
    # print coef, intercept
    print '%s = [%s, %0.6f]' % (ticker, ", ".join(map(lambda x: '%0.6f' % x, coef)), intercept)
    # Export the model

    with open('models/test_model.mod', 'w') as f_handle:
        f_handle.write('stocks = ["%s"];\n' % ticker)
        f_handle.write('stockfiles = ["models/%s_data.dat"];\n' % ticker)
        f_handle.write('factors = ["%s"];\n' %
                       '", "'.join(sorted(benchmarks.keys())))
        f_handle.write('factorfiles = ["%s"];\n' % '", "'.join(
            [os.path.join('models', '%s.dat' % x) for x in sorted(benchmarks.keys())]))
        f_handle.write('%s = [%s, %0.6f]' %
                       (ticker, ", ".join(map(lambda x: '%0.6f' % x, coef)), intercept))

    with open(os.path.join('models', '%s_data.dat'%ticker), 'w') as f_handle:
        f_handle.write(' '.join(map(lambda x: '%0.5f'%x, data)))

    for benchmark in benchmarks:
        with open(os.path.join('models', '%s.dat'%benchmark), 'w') as f_handle:
            f_handle.write(' '.join(map(lambda x: '%0.5f'%x, [benchmarks[benchmark][di] for di in dates])))


if __name__ == '__main__':
    model_stock('WMT', '2014-03-01', '2016-03-01')
