import os
import utils
from matplotlib import pyplot as plt
import numpy as np

TEST_SIZES = [100, 1000, 2000, 3000, 5000, 10000, 20000, 30000, 40000, 50000]  #100000, 500000, 1000000, 2000000
CUTOFS = {'SelectionSort': 10000}  #
NUM_PER_SIZE = 3                                     #numbers of repetitions
SOURCES = 'sources'
BINS = 'bins'
TESTS = 'tests'
RESULTS = 'results'
TIMES = 'times'
VERBOSE = False

utils.create_tests(TESTS, TEST_SIZES, NUM_PER_SIZE)
utils.compile_sources(SOURCES, BINS, v=VERBOSE)

for algo in os.listdir(BINS):
    for ts in TEST_SIZES:
        if algo in CUTOFS and ts > CUTOFS[algo]: continue
        for tn in range(NUM_PER_SIZE):
            utils.run_algo(BINS, TESTS, RESULTS, TIMES, ts, tn, algo, v=VERBOSE)

times = utils.read_times(TIMES)

for algo in times:
    x = times[algo]['x']
    y = times[algo]['y']

    # fig, ax = plt.subplots()
    plt.plot(x, y, '.', label=algo)

    plt.xlabel("n")
    plt.ylabel("seconds")
    plt.legend()
    plt.show()

from sklearn.linear_model import LinearRegression

times = utils.read_times(TIMES)

functions = {'SelectionSort': lambda x: x * x}
other = lambda x: x * np.log(x)

for algo in times:
    if algo in CUTOFS:
        max_x = CUTOFS[algo]
    else:
        max_x = max(TEST_SIZES)
    if algo in functions:
        fun = functions[algo]
    else:
        fun = other
    x = times[algo]['x']
    y = times[algo]['y']
    plt.plot(x, y, '.')

    Y = np.array(y)
    X = np.array([[a, fun(a)] for a in x])
    model = LinearRegression().fit(X, Y)

    f_x = np.array([a for a in range(1, max_x, 10)])
    tmp_x = np.array([[a, fun(a)] for a in range(1, max_x, 10)])
    f_y = np.matmul(tmp_x, model.coef_) + model.intercept_
    plt.plot(f_x, f_y, label=algo)

    plt.legend()
    plt.show()

