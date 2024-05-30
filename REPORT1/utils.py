import os
import random
import statistics
import subprocess
import math
from matplotlib import pyplot as plt
import numpy as np
import collections


peak = 0


def mkdir(directory):
    try:
        os.mkdir(directory)
    except FileExistsError:
        pass


def generate(n, min, max, step):
    arm = []
    global peak
    for i in range(n + 1):
        number = int(random.randrange(min, max, step))
        arm.append(number)
    peak = arm[len(arm) - 1]
    return arm


def generateV(maxim):
    arm1 = generate(n - 1, 0, maxim, 2)
    arm2 = generate(n - 1, 1, peak - 1, 2)
    arm1.sort(reverse=True)
    arm2.sort()
    numbers = arm1 + arm2
    return numbers


def generateA(maxim):
    arm1 = generate(n - 1, 1, maxim, 2)
    arm2 = generate(n - 1, 0, peak - 1, 2)
    arm1.sort()
    arm2.sort(reverse=True)
    numbers = arm1 + arm2
    return numbers


def generateInc(maxim):
    numbers = generate(2 * n - 1, 0, maxim, 1)
    numbers.sort()
    return numbers


def generateDec(maxim):
    numbers = generate(2 * n - 1, 0, maxim, 1)
    numbers.sort(reverse=True)
    return numbers


def generateRand(maxim):
    numbers = generate(2 * n - 1, 0, maxim, 1)
    return numbers


def create_tests(test_dir, test_sizes, num_per_size, maxim):
    mkdir(test_dir)
    global n
    maxim = 10 * maxim
    for ts in test_sizes:
        n = math.floor(ts / 2)
        for tn in range(num_per_size):
            f = open('{}/{}_{}_{}.in'.format(test_dir, ts, tn, 'A'), 'w')

            # num = [random.randint(0, 10000) for _ in range(ts)]
            num = generateA(maxim)

            f.write('{}\n'.format(ts))
            f.write('{}'.format('\n'.join([str(x) for x in num])))
            f.close()
        for tn in range(num_per_size):
            f = open('{}/{}_{}_{}.in'.format(test_dir, ts, tn, 'V'), 'w')

            # num = [random.randint(0, 10000) for _ in range(ts)]
            num = generateV(maxim)

            f.write('{}\n'.format(ts))
            f.write('{}'.format('\n'.join([str(x) for x in num])))
            f.close()
        for tn in range(num_per_size):
            f = open('{}/{}_{}_{}.in'.format(test_dir, ts, tn, 'Inc'), 'w')

            # num = [random.randint(0, 10000) for _ in range(ts)]
            num = generateInc(maxim)

            f.write('{}\n'.format(ts))
            f.write('{}'.format('\n'.join([str(x) for x in num])))
            f.close()
        for tn in range(num_per_size):
            f = open('{}/{}_{}_{}.in'.format(test_dir, ts, tn, 'Dec'), 'w')

            # num = [random.randint(0, 10000) for _ in range(ts)]
            num = generateDec(maxim)

            f.write('{}\n'.format(ts))
            f.write('{}'.format('\n'.join([str(x) for x in num])))
            f.close()
        for tn in range(num_per_size):
            f = open('{}/{}_{}_{}.in'.format(test_dir, ts, tn, 'Rand'), 'w')

            # num = [random.randint(0, 10000) for _ in range(ts)]
            num = generateRand(maxim)

            f.write('{}\n'.format(ts))
            f.write('{}'.format('\n'.join([str(x) for x in num])))
            f.close()


def compile_sources(sources, bins, v=False):
    mkdir(bins)
    for code in os.listdir(sources):
        bin_name = code.split('.')[0]
        ext = code.split('.')[1]
        if ext == 'cpp':
            command = ['g++', '{}/{}'.format(sources, code), '-o', '{}/{}'.format(bins, bin_name)]
        else:
            command = ['gcc', '{}/{}'.format(sources, code), '-o', '{}/{}'.format(bins, bin_name)]
        a = subprocess.run(command, capture_output=True)
        print('Executing: {}'.format(' '.join(command)))
        if v:
            print(a.stdout.decode('utf-8'))
            print(a.stderr.decode('utf-8'))


def run_algo(bins, test_dir, result_dir, time_dir, ts, tn, algo, string, v=False):
    mkdir(result_dir)
    mkdir(time_dir)
    f_in_n = '{}/{}_{}_{}.in'.format(test_dir, ts, tn, string)
    f_out_n = '{}/{}_{}_{}_{}.out'.format(result_dir, algo, ts, tn, string)
    f_err_n = '{}/{}_{}_{}_{}.time'.format(time_dir, algo, ts, tn, string)
    f_in = open(f_in_n, 'r')
    f_out = open(f_out_n, 'w')
    f_err = open(f_err_n, 'w')
    command = ['time', '-p', '{}/{}'.format(bins, algo)]  # '-f  user  %U\nsystem  %S\nelapsed %e'
    subprocess.run(command, stdin=f_in, stdout=f_out, stderr=f_err)
    if v:
        print(command, f'in={f_in_n}, out={f_out_n}, err={f_err_n}')
    f_in.close()
    f_out.close()
    f_err.close()


# def read_times(times):
#     res = {}
#     for file in os.listdir(times):
#         algo, size, _, string = file.split('_')
#
#         if algo not in res:
#             res[algo] = {'x': [], 'y': []}
#
#         res[algo]['x'].append(int(size))
#
#         f = open(times + '/' + file, 'r')
#         for line in f:
#             if 'user' in line:
#                 y = line.split()[1]
#                 # y = float(m)*60 + float(s)
#
#         f.close()
#         res[algo]['y'].append(float(y))
#     return res


def times_algo_from_dane(times):
    time_dict = {}
    errors = []
    for file in os.listdir(times):
        algo, size, num, s = file.split('_')
        string = s.split(".")[0]
        size = int(size)

        f = open(times + '/' + file, 'r')
        for line in f:
            try:
                if 'user' in line:
                    user_time = line.split()[1]
            except Exception as ex:
                print(ex)

        f.close()

        if (algo, size, string) not in time_dict:
            time_dict[(algo, size, string)] = []
        time_dict[(algo, size, string)].append(float(user_time))

    sorted_data = sorted(time_dict.items(), key=lambda item: item[0][1])
    sorted_data = collections.OrderedDict(sorted_data)
    # print("SORTED DATA", sorted_data)

    for keys, lists in sorted_data.items():

        mean = statistics.mean(lists)
        # print("Key: ", keys, "-----lists: ->  ", lists, " mean: ", mean)
        mean = round(mean, 3)
        sorted_data[keys] = mean

        std_deviation = statistics.stdev(lists)
        error = std_deviation / (len(lists) ** 0.5)
        errors.append(error)

    res = lists_of_points(sorted_data, errors)
    return res


def lists_of_points(times, errors):
    algo_str = {}
    i = 0
    for algo, size, string in times:
        if (algo, string) not in algo_str:
            algo_str[algo, string] = {'x': [], 'y': [], 'er': []}
        algo_str[algo, string]['x'].append(size)
        algo_str[algo, string]['y'].append(
            times[algo, size, string])  # {('ShellSort', 'V'): {'x': [5000, 1000, 100, 2000, 3000], ...}
        algo_str[algo, string]['er'].append(errors[i])
        i += 1

    return algo_str

def plot_string_algo(type_string, times):
    for t in type_string:
        for algo, string in times:
            if t == string:
                x = np.array(times[algo, string]['x'])
                y = np.array(times[algo, string]['y'])
                errors = np.array(times[algo, string]['er'])
                plt.plot(x, y, '-', label=string + " gen, " + algo)
                plt.errorbar(x, y, yerr=errors, fmt='.', capsize=5)

                plt.xlabel("ilość danych wejściowych")
                plt.ylabel("sekundy (s)")
                plt.yscale('log')

        plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)
        plt.show()


def plot_algo_size(times):
    algo_list = []
    for algo, string in times:
        if algo not in algo_list:
            algo_list.append(algo)

    for t in algo_list:
        for algo, string in times:
            if t == algo:
                x = np.array(times[algo, string]['x'])
                y = np.array(times[algo, string]['y'])
                errors = np.array(times[algo, string]['er'])
                plt.plot(x, y, '-', label=string + " gen, " + algo)
                plt.errorbar(x, y, yerr=errors, fmt='.', capsize=5)
                plt.xlabel("ilość danych wejściowych")
                plt.ylabel("sekundy (s)")
                plt.yscale('log')
        plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)
        plt.show()



# def plot_conf_s(type_strings, test_sizes, times, cutofs, func, other):
#     for t in type_strings:
#         for algo, string in times:
#
#             if algo in cutofs:
#                 max_x = cutofs[algo]
#             else:
#                 max_x = max(test_sizes)
#             if algo in func:
#                 fun = func[algo]
#             else:
#                 fun = other
#
#             if t == string:
#                 x = times[algo, string]['x']
#                 y = times[algo, string]['y']
#                 plt.plot(x, y, '.')
#
#                 Y = np.array(y)
#                 X = np.array([[a, fun(a)] for a in x])
#                 model = LinearRegression().fit(X, Y)
#
#                 f_x = np.array([a for a in range(1, max_x, 10)])
#                 tmp_x = np.array([[a, fun(a)] for a in range(1, max_x, 10)])
#                 f_y = np.matmul(tmp_x, model.coef_) + model.intercept_
#                 plt.plot(f_x, f_y, label=algo + ", " + string)
#
#         plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)
#         plt.show()


# def plot_conf_alg(test_sizes, times, cutofs, func, other):
#     algo_list = []
#     for algo, string in times:
#         if algo not in algo_list:
#             algo_list.append(algo)
#
#     for t in algo_list:
#         for algo, string in times:
#
#             if algo in cutofs:
#                 max_x = cutofs[algo]
#             else:
#                 max_x = max(test_sizes)
#             if algo in func:
#                 fun = func[algo]
#             else:
#                 fun = other
#
#             if t == algo:
#                 x = times[algo, string]['x']
#                 y = times[algo, string]['y']
#                 plt.plot(x, y, '.')
#
#                 Y = np.array(y)
#                 X = np.array([[a, fun(a)] for a in x])
#                 model = LinearRegression().fit(X, Y)
#
#                 f_x = np.array([a for a in range(1, max_x, 10)])
#                 tmp_x = np.array([[a, fun(a)] for a in range(1, max_x, 10)])
#                 f_y = np.matmul(tmp_x, model.coef_) + model.intercept_
#                 plt.plot(f_x, f_y, label=algo + ", " + string)
#
#         plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)
#         plt.show()













