import os
import subprocess
from matplotlib import pyplot as plt
import numpy as np
from generators import generate_knapsack_data

from IPython.display import display


def mkdir(directory):
    try:
        os.mkdir(directory)
    except FileExistsError:
        pass


def create_tests(test_dir, test_sizes, capacity_sizes):
    if not os.path.exists(test_dir):
        os.makedirs(test_dir)
    if not os.path.exists(f"{test_dir}/const_capacity"):
        os.makedirs(f"{test_dir}/const_capacity")
    if not os.path.exists(f"{test_dir}/const_num"):
        os.makedirs(f"{test_dir}/const_num")
    if not os.path.exists(f"{test_dir}/3d"):
        os.makedirs(f"{test_dir}/3d")

    # const capacity
    for size in test_sizes:
        result = generate_knapsack_data(size)
        capacity = 50
        with open(f'{test_dir}/const_capacity/{size}.in', 'w') as f:
            f.write(f'{size} {capacity}\n')
            f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result))
            f.close()

    # const number
    for cap in capacity_sizes:
        size = 25
        result = generate_knapsack_data(size)
        with open(f'{test_dir}/const_num/{cap}.in', 'w') as f:
            f.write(f'{size} {cap}\n')
            f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result))
            f.close()

    # capacity, numbers
    for size in test_sizes:
        for cap in capacity_sizes:
            result = generate_knapsack_data(size)
            with open(f'{test_dir}/3d/{size}_{cap}.in', 'w') as f:
                f.write(f'{size} {cap}\n')
                f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result))
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


def run_algo(bins, test_dir, result_dir, test_sizes, capacity_sizes, v=False):
    mkdir(result_dir)
    # if not os.path.exists(f"{result_dir}/const_capacity"):
    #     os.makedirs(f"{result_dir}/const_capacity")
    if not os.path.exists(f"{result_dir}/const_num"):
        os.makedirs(f"{result_dir}/const_num")
    if not os.path.exists(f"{result_dir}/3d"):
        os.makedirs(f"{result_dir}/3d")

    # for ts in test_sizes:
    #     f_in_n1 = f'{test_dir}/const_capacity/{ts}.in'
    #     f_out_n1 = f'{result_dir}/const_capacity/{ts}.out'
    #     f_in = open(f_in_n1, 'r')
    #     f_out = open(f_out_n1, 'w')
    #     command = [f'{bins}/knapsnack']
    #     subprocess.run(command, stdin=f_in, stdout=f_out)
    #     if v:
    #         print(command, f'in={f_in_n1}, out={f_out_n1}')
    #     f_in.close()
    #     f_out.close()

    for cap in capacity_sizes:
        f_in_n1 = f'{test_dir}/const_num/{cap}.in'
        f_out_n1 = f'{result_dir}/const_num/{cap}.out'
        f_in = open(f_in_n1, 'r')
        f_out = open(f_out_n1, 'w')
        command = [f'{bins}/knapsnack']
        subprocess.run(command, stdin=f_in, stdout=f_out)
        if v:
            print(command, f'in={f_in_n1}, out={f_out_n1}')
        f_in.close()
        f_out.close()

    for ts in test_sizes:
        for cap in capacity_sizes:
            f_in_n1 = f'{test_dir}/3d/{ts}_{cap}.in'
            f_out_n1 = f'{result_dir}/3d/{ts}_{cap}.out'
            f_in = open(f_in_n1, 'r')
            f_out = open(f_out_n1, 'w')
            command = [f'{bins}/knapsnack']
            subprocess.run(command, stdin=f_in, stdout=f_out)
            if v:
                print(command, f'in={f_in_n1}, out={f_out_n1}')
            f_in.close()
            f_out.close()


def read_results(results):
    global time
    result_cap_const = {'Brute_Force': {'x': [], 'y': []},
                        'Dynamic': {'x': [], 'y': []},
                        'Greedy': {'x': [], 'y': []}}

    result_num_const = {'Brute_Force': {'x': [], 'y': []},
                        'Dynamic': {'x': [], 'y': []},
                        'Greedy': {'x': [], 'y': []}}

    result_num_cap = {'Brute_Force': {'x': [], 'y': [], 'z': []},
                      'Dynamic': {'x': [], 'y': [], 'z': []},
                      'Greedy': {'x': [], 'y': [], 'z': []}}
    for dir in os.listdir(f"{results}"):
        for file in os.listdir(f"{results}/{dir}"):
            if "const_capacity" in dir:
                size = int(file.split('.')[0])
                f = open(results + '/' + "const_capacity" + '/' + file, 'r')
                time_brute = 0
                time_dyn = 0
                time_grd = 0
                for line in f:
                    if "brute" in line:
                        time_brute = float(line.split(' ')[3])
                        result_cap_const['Brute_Force']['y'].append(time_brute)
                        result_cap_const['Brute_Force']['x'].append(size)
                    if "dynamic" in line:
                        time_dyn = float(line.split(' ')[3])
                        result_cap_const['Dynamic']['y'].append(time_dyn)
                        result_cap_const['Dynamic']['x'].append(size)
                    if "greedy" in line:
                        time_grd = float(line.split(' ')[3])
                        result_cap_const['Greedy']['y'].append(time_grd)
                        result_cap_const['Greedy']['x'].append(size)
                f.close()

            if "const_num" in dir:
                cap = int(file.split('.')[0])
                f = open(results + '/' + "const_num" + '/' + file, 'r')
                time_brute = 0
                time_dyn = 0
                time_grd = 0
                for line in f:
                    if "brute" in line:
                        time_brute = float(line.split(' ')[3])
                        result_num_const['Brute_Force']['y'].append(time_brute)
                        result_num_const['Brute_Force']['x'].append(cap)
                    if "dynamic" in line:
                        time_dyn = float(line.split(' ')[3])
                        result_num_const['Dynamic']['y'].append(time_dyn)
                        result_num_const['Dynamic']['x'].append(cap)
                    if "greedy" in line:
                        time_grd = float(line.split(' ')[3])
                        result_num_const['Greedy']['y'].append(time_grd)
                        result_num_const['Greedy']['x'].append(cap)
                f.close()

            if "3d" in dir:
                size = int(file.split('_')[0])
                cap = int(file.split('_')[1].split('.')[0])
                f = open(results + '/' "3d" + '/' + file, 'r')
                time_brute = 0
                time_dyn = 0
                time_grd = 0
                for line in f:
                    if "brute" in line:
                        time_brute = float(line.split(' ')[3])
                        result_num_cap['Brute_Force']['z'].append(time_brute)
                        result_num_cap['Brute_Force']['y'].append(cap)
                        result_num_cap['Brute_Force']['x'].append(size)
                    if "dynamic" in line:
                        time_dyn = float(line.split(' ')[3])
                        result_num_cap['Dynamic']['z'].append(time_dyn)
                        result_num_cap['Dynamic']['y'].append(cap)
                        result_num_cap['Dynamic']['x'].append(size)
                    if "greedy" in line:
                        time_grd = float(line.split(' ')[3])
                        result_num_cap['Greedy']['z'].append(time_grd)
                        result_num_cap['Greedy']['y'].append(cap)
                        result_num_cap['Greedy']['x'].append(size)
                f.close()

    # for algorithm, values in result_cap_const.items():
    #     for key in values:
    #         values[key].sort()
    #
    # for algorithm, values in result_num_const.items():
    #     for key in values:
    #         values[key].sort()
    #
    # for algorithm, values in result_num_cap.items():
    #     for key in values:
    #         values[key].sort()

    return result_cap_const, result_num_const, result_num_cap


# each algo in one graph
def plot_graf_t_from_n(dictionary):
    for algo in dictionary:
        if algo == "Brute_Force":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            # print(algo, x, y)
            plt.plot(x, y, label="Brute_Force", linestyle='-', marker='o')

        if algo == "Dynamic":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            # print(algo, x, y)
            plt.plot(x, y, label="Dynamic", linestyle='--', marker='s')

        if algo == "Greedy":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            # print(algo, x, y)
            plt.plot(x, y, label="Greedy", linestyle='-.', marker='x')

    plt.xlabel("quantity of items")
    plt.ylabel("seconds (s)")
    plt.yscale('log')
    plt.legend()

    plt.text(1.05, 0.5, 'For capacity = 50', fontsize=12, rotation=90, ha='center', va='center',
             transform=plt.gca().transAxes)

    plt.show()


# for every algo
def plot_graf_t_from_capacity(dictionary):
    for algo in dictionary:
        if algo == "Brute_Force":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            # print(algo, x, y)
            plt.plot(x, y, label="Brute_Force", linestyle='-', marker='o')

        if algo == "Dynamic":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            # print(algo, x, y)
            plt.plot(x, y, label="Dynamic", linestyle='--', marker='s')

        if algo == "Greedy":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            # print(algo, x, y)
            plt.plot(x, y, label="Greedy", linestyle='-.', marker='x')

        plt.xlabel("capacity")
        plt.ylabel("seconds (s)")
        # plt.yscale('log')
        plt.legend()

        plt.text(1.05, 0.5, 'For quantity of items = 25', fontsize=12, rotation=90, ha='center', va='center',
                 transform=plt.gca().transAxes)

        plt.show()


def plot_graf3D(res):
    for algorithm in res:
        fig = plt.figure(figsize=(8, 6))  # Setting the figure size
        ax = fig.add_subplot(111, projection='3d')

        x = sorted(np.array(res[algorithm]['x']))
        y = sorted(np.array(res[algorithm]['y']))
        z = sorted(np.array(res[algorithm]['z']))

        # Create grid for X and Z
        X_unique = np.unique(x)
        Z_unique = np.unique(z)
        X, Z = np.meshgrid(X_unique, Z_unique)
        Y = np.zeros_like(X, dtype=float)

        # Fill the Y grid with corresponding y values
        for i in range(len(x)):
            xi = np.where(X_unique == x[i])[0][0]
            zi = np.where(Z_unique == z[i])[0][0]
            Y[zi, xi] = y[i]  # Use y[i] directly, or use np.log10(y[i]) if y[i] > 0 else np.nan for log scale

        # Plot the surface
        ax.plot_surface(X, Z, Y, cmap='viridis')

        ax.set_xlabel('Quantity of Items')
        ax.set_ylabel('Capacity')
        ax.set_zlabel('Time (s)')

        plt.title(f"{algorithm}")
        plt.tight_layout()
        plt.show()
