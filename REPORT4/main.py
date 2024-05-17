import multiprocessing
import os
import subprocess
from tqdm.notebook import tqdm
from matplotlib import pyplot as plt
import networkx as nx
import random
from generators import generate_undirected_hamiltonian_cyclic_graph
from generators import generate_hamiltonian_cyclic_digraph


def mkdir(directory):
    try:
        os.mkdir(directory)
    except FileExistsError:
        pass

def create_tests(test_dir, test_sizes, saturations):
    if not os.path.exists(test_dir):
        os.makedirs(test_dir)
    if not os.path.exists(f"{test_dir}/DA"):  # directed acyclic
        os.makedirs(f"{test_dir}/DA")
    if not os.path.exists(f"{test_dir}/DC"):  # directed acyclic
        os.makedirs(f"{test_dir}/DC")
    if not os.path.exists(f"{test_dir}/UDA"):  # directed acyclic
        os.makedirs(f"{test_dir}/UDA")
    if not os.path.exists(f"{test_dir}/UDC"):  # directed acyclic
        os.makedirs(f"{test_dir}/UDC")

    for saturation in saturations:
        # for size in test_sizes:
        #         result = generate_directed_acyclic_graph(size, saturation)
        #
        #         with open(f'{test_dir}/{"DA"}/{saturation}_{size}.in', 'w') as f:
        #             f.write(f'{size} {int(size * (size - 1))}\n')
        #             f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result))
        #             f.close()

        for size in test_sizes:
            result = generate_hamiltonian_cyclic_digraph(size, saturation)

            with open(f'{test_dir}/{"DC"}/{saturation}_{size}.in', 'w') as f:
                f.write(f'{size} {int(saturation / 100 * size * (size - 1))}\n')
                if result == "0 0":
                    f.write("0 0\n")
                else:
                    f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result))
                f.close()

        # for size in test_sizes:
        #     result = generate_undirected_acyclic_graph(size, saturation)
        #
        #     with open(f'{test_dir}/{"UDA"}/{saturation}_{size}.in', 'w') as f:
        #         f.write(f'{size} {int(size * (size - 1) / 2)}\n')
        #         f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result))
        #         f.close()
        #
        for size in test_sizes:
            result = generate_undirected_hamiltonian_cyclic_graph(size, saturation)

            with open(f'{test_dir}/{"UDC"}/{saturation}_{size}.in', 'w') as f:
                f.write(f'{size} {int(saturation / 100 * size * (size - 1) / 2)}\n')
                if result == "0 0":
                    f.write("0 0\n")
                else:
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


def run_algo(bins, test_dir, result_dir, test_sizes, saturations, v=False):
    mkdir(result_dir)
    if not os.path.exists(f"{result_dir}/DA"):
        os.makedirs(f"{result_dir}/DA")
    if not os.path.exists(f"{result_dir}/DC"):
        os.makedirs(f"{result_dir}/DC")
    if not os.path.exists(f"{result_dir}/UDA"):
        os.makedirs(f"{result_dir}/UDA")
    if not os.path.exists(f"{result_dir}/UDC"):
        os.makedirs(f"{result_dir}/UDC")

    for algo in os.listdir(bins):
        for saturation in saturations:
            for ts in test_sizes:
                if "Robertsa-Floresa-directed" in algo:
                    # f_in_n1 = f'{test_dir}/DA/{saturation}_{ts}.in'
                    # f_out_n1 = f'{result_dir}/DA/{algo}_{saturation}_{ts}.out'
                    # f_in = open(f_in_n1, 'r')
                    # f_out = open(f_out_n1, 'w')
                    # command = [f'{bins}/{algo}']
                    # subprocess.run(command, stdin=f_in, stdout=f_out)
                    # if v:
                    #     print(command, f'in={f_in_n1}, out={f_out_n1}')
                    # f_in.close()
                    # f_out.close()

                    f_in_n2 = f'{test_dir}/DC/{saturation}_{ts}.in'
                    f_out_n2 = f'{result_dir}/DC/{algo}_{saturation}_{ts}.out'
                    f_in = open(f_in_n2, 'r')
                    f_out = open(f_out_n2, 'w')
                    command = [f'{bins}/{algo}']
                    subprocess.run(command, stdin=f_in, stdout=f_out)
                    if v:
                        print(command, f'in={f_in_n2}, out={f_out_n2}')
                    f_in.close()
                    f_out.close()

                if "Robertsa-Floresa-undirected" in algo:
                    # f_in_n1 = f'{test_dir}/UDA/{saturation}_{ts}.in'
                    # f_out_n1 = f'{result_dir}/UDA/{algo}_{saturation}_{ts}.out'
                    # f_in = open(f_in_n1, 'r')
                    # f_out = open(f_out_n1, 'w')
                    # command = [f'{bins}/{algo}']
                    # subprocess.run(command, stdin=f_in, stdout=f_out)
                    # if v:
                    #     print(command, f'in={f_in_n1}, out={f_out_n1}')
                    # f_in.close()
                    # f_out.close()

                    f_in_n2 = f'{test_dir}/UDC/{saturation}_{ts}.in'
                    f_out_n2 = f'{result_dir}/UDC/{algo}_{saturation}_{ts}.out'
                    f_in2 = open(f_in_n2, 'r')
                    f_out2 = open(f_out_n2, 'w')
                    command = [f'{bins}/{algo}']
                    subprocess.run(command, stdin=f_in2, stdout=f_out2)
                    if v:
                        print(command, f'in={f_in_n2}, out={f_out_n2}')
                    f_in2.close()
                    f_out2.close()


def read_results(results):
    res_DA = {'Robertsa-Floresa': {'x': [], 'y': [], 'z': []}, 'Fleury': {'x': [], 'y': [], 'z': []}}
    res_UDA = {'Robertsa-Floresa': {'x': [], 'y': [], 'z': []}, 'Fleury': {'x': [], 'y': [], 'z': []}}
    res_DC = {'Robertsa-Floresa': {'x': [], 'y': [], 'z': []}, 'Fleury': {'x': [], 'y': [], 'z': []}}
    res_UDC = {'Robertsa-Floresa': {'x': [], 'y': [], 'z': []}, 'Fleury': {'x': [], 'y': [], 'z': []}}
    for dir in os.listdir(results):
        for file in os.listdir(f"{results}/{dir}"):
            algo, saturation, size = file.split('_')
            size = size.split('.')[0]
            f = open(results + '/' + dir + '/' + file, 'r')
            line = f.read()
            if dir == "DA":
                res_DA['Robertsa-Floresa']['x'].append(size)
                res_DA['Fleury']['x'].append(size)
                time = float(line.split()[-2])  # TODO time from file
                if 'RF' in algo:
                    res_DA['Robertsa-Floresa']['y'].append(time)
                    res_DA['Robertsa-Floresa']['z'].append(float(saturation))
                if 'Fleury' in algo:
                    res_DA['Robertsa-Floresa']['y'].append(time)
                    res_DA['Robertsa-Floresa']['z'].append(float(saturation))

            if dir == "DC":
                res_DC['Robertsa-Floresa']['x'].append(size)
                res_DC['Fleury']['x'].append(size)
                time = float(line.split()[-2])  # TODO time from file
                if 'RF' in algo:
                    res_DC['Robertsa-Floresa']['y'].append(time)
                    res_DC['Robertsa-Floresa']['z'].append(float(saturation))
                if 'Fleury' in algo:
                    res_DC['Robertsa-Floresa']['y'].append(time)
                    res_DC['Robertsa-Floresa']['z'].append(float(saturation))

            if dir == "UDA":
                res_UDA['Robertsa-Floresa']['x'].append(size)
                res_UDA['Fleury']['x'].append(size)
                time = float(line.split()[-2])  # TODO time from file
                if 'RF' in algo:
                    res_UDA['Robertsa-Floresa']['y'].append(time)
                    res_UDA['Robertsa-Floresa']['z'].append(float(saturation))
                if 'Fleury' in algo:
                    res_UDA['Robertsa-Floresa']['y'].append(time)
                    res_UDA['Robertsa-Floresa']['z'].append(float(saturation))

            if dir == "UDC":
                res_UDC['Robertsa-Floresa']['x'].append(size)
                res_UDC['Fleury']['x'].append(size)
                time = float(line.split()[-2])  # TODO time from file
                if 'RF' in algo:
                    res_UDC['Robertsa-Floresa']['y'].append(time)
                    res_UDC['Robertsa-Floresa']['z'].append(float(saturation))
                if 'Fleury' in algo:
                    res_UDC['Robertsa-Floresa']['y'].append(time)
                    res_UDC['Robertsa-Floresa']['z'].append(float(saturation))
            f.close()
    return res_DA, res_UDA, res_DC, res_UDC


def plot_graf(dictionary):
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)  # 1 строка, 2 столбца, первый график
    for algo in dictionary:
        if algo == "TwM" or algo == "KwM":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            # print(algo, x, y)
            plt.plot(x, y, label=algo)

    # plt.gca().yaxis.set_major_formatter(ticker.FormatStrFormatter('%.2e'))
    plt.xlabel("ilość danych wejściowych")
    plt.ylabel("milisekundy (ms)")
    plt.yscale('log')
    plt.legend()
    # plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)

    plt.subplot(1, 2, 2)  # 1 строка, 2 столбца, второй график
    for algo in dictionary:
        if algo == "TwL" or algo == "KwL":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            # print(algo, x, y)
            plt.plot(x, y, label=algo)

    # plt.gca().yaxis.set_major_formatter(ticker.FormatStrFormatter('%.2e'))
    plt.xlabel("ilość danych wejściowych")
    plt.ylabel("milisekundy (ms)")
    plt.yscale('log')
    plt.legend()
    # plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)

    plt.show()


def plot_graf_method(dictionary):
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)  # 1 строка, 2 столбца, первый график
    for algo in dictionary:
        if algo == "KwL" or algo == "KwM":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            plt.plot(x, y, label=algo)

    # plt.gca().yaxis.set_major_formatter(ticker.FormatStrFormatter('%.2e'))
    plt.xlabel("ilość danych wejściowych")
    plt.ylabel("milisekundy (ms)")
    plt.yscale('log')
    plt.legend()
    # plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)

    plt.subplot(1, 2, 2)  # 1 строка, 2 столбца, второй график
    for algo in dictionary:
        if algo == "TwL" or algo == "TwM":
            x = sorted(dictionary[algo]['x'])
            y = sorted(dictionary[algo]['y'])
            plt.plot(x, y, label=algo)

    # plt.gca().yaxis.set_major_formatter(ticker.FormatStrFormatter('%.2e'))
    plt.xlabel("ilość danych wejściowych")
    plt.ylabel("milisekundy (ms)")
    plt.yscale('log')
    plt.legend()
    # plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)

    plt.show()
