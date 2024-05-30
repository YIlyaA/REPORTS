import os
import random
import subprocess
import math
import matplotlib.ticker as ticker
from matplotlib import pyplot as plt
import numpy as np

peak = 0


def mkdir(directory):
    try:
        os.mkdir(directory)
    except FileExistsError:
        pass


def create_tests(test_dir, test_sizes, maxim, minimum):
    mkdir(test_dir)
    global n
    maxim = 10 * maxim
    for ts in test_sizes:
        n = math.floor(ts / 2)
        f = open('{}/{}.in'.format(test_dir, ts), 'w')

        num = [random.randint(minimum, maxim) for _ in range(ts)]
        num = sorted(num, reverse=True)

        f.write('{}\n'.format(ts))
        f.write('{}'.format(' '.join([str(x) for x in num])))
        f.close()


def compile_sources(sources, bins, v=False):
    mkdir(bins)
    for code in os.listdir(sources):
        bin_name = code.split('.')[0]
        ext = code.split('.')[1]
        if ext == 'cpp':
            command = ['g++', '{}/{}'.format(sources, code), '-o', '{}/{}'.format(bins, bin_name)]
        else:
            command = ['gcc', '{}/{}'.format(sources, code), '-o', '{}/{}'.format(bins, bin_name), '-lm']
        a = subprocess.run(command, capture_output=True)
        print('Executing: {}'.format(' '.join(command)))
        if v:
            print(a.stdout.decode('utf-8'))
            print(a.stderr.decode('utf-8'))


def run_algo(bins, test_dir, result_dir, ts, algo, v=False):
    mkdir(result_dir)
    f_in_n = '{}/{}.in'.format(test_dir, ts)
    f_out_n = '{}/{}_{}.out'.format(result_dir, algo, ts)
    f_in = open(f_in_n, 'r')
    f_out = open(f_out_n, 'w')
    command = ['{}/{}'.format(bins, algo)]
    subprocess.run(command, stdin=f_in, stdout=f_out)
    if v:
        print(command, f'in={f_in_n}, out={f_out_n}')
    f_in.close()
    f_out.close()


def read_results(results):
    res_create = {}
    res_find_min = {}
    res_balance = {"BST": {'x': [], 'y': []}}
    for file in os.listdir(results):
        algo, size = file.split('_')
        size = size.split('.')[0]

        if algo not in res_create:
            res_create[algo] = {'x': [], 'y': []}
        if size not in res_create[algo]['x']:
            res_create[algo]['x'].append(int(size))

        if algo not in res_find_min:
            res_find_min[algo] = {'x': [], 'y': []}
        if size not in res_find_min[algo]['x']:
            res_find_min[algo]['x'].append(int(size))

        if algo == "BST" and size not in res_balance[algo]['x']:
            res_balance[algo]['x'].append(int(size))

        f = open(results + '/' + file, 'r')
        for line in f:
            if 'create_time' in line:
                y = line.split()[1]
                res_create[algo]['y'].append(float(y))
            if 'find_min_time' in line:
                y = line.split()[1]
                res_find_min[algo]['y'].append(float(y))
            if 'balance_time' in line and algo == "BST":
                y = line.split()[1]
                res_balance[algo]['y'].append(float(y))

        f.close()
    return res_create, res_find_min, res_balance


def plot_graf(dictionary, marker, linestyle, undtitle=False):
    # print(dictionary)
    for algo in dictionary:
        if algo == "BST":
            linestyle = '--'
        x = sorted(dictionary[algo]['x'])
        y = sorted(dictionary[algo]['y'])
        plt.plot(x, y, marker=marker, linestyle=linestyle, label=algo)

        # plt.gca().yaxis.set_major_formatter(ticker.FormatStrFormatter('%.2e'))
        plt.xlabel("ilość danych wejściowych")
        plt.ylabel("sekundy (s)")
        plt.yscale('log')
    if undtitle:
        plt.figtext(0.5, -0.02, "*Dla małych liczb czas dla AVL poniżej dolnej granicy pomiaru.*",
                    ha='center',
                    fontsize=10,
                    fontstyle='italic')
    plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)
    plt.show()
