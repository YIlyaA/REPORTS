import multiprocessing
import os
import subprocess
from tqdm.notebook import tqdm
from matplotlib import pyplot as plt
import networkx as nx
import random


def mkdir(directory):
    try:
        os.mkdir(directory)
    except FileExistsError:
        pass


def generate_acyclic_graph_batch(test_size, output_queue):
    num_edges = int(test_size * (test_size - 1) / 2)
    desc = f"Generating tests for {test_size}"
    result = generate_acyclic_graph((test_size, num_edges, desc))
    output_queue.put((test_size, result))


def generate_acyclic_graph(args):
    num_nodes, num_edges, desc = args
    graph = nx.DiGraph()
    nodes = list(range(num_nodes))
    graph.add_nodes_from(nodes)

    edges_added = 0
    # progress_interval = num_edges // 100  # Определяем интервал обновления прогресса

    for _ in range(num_edges):
        node_from = random.choice(nodes)
        node_to = random.choice(nodes)

        if node_from != node_to:
            graph.add_edge(node_from, node_to)
            edges_added += 1
            # if edges_added % progress_interval == 0:
            #     tqdm.write(f"{desc}: {edges_added / num_edges * 100:.2f}% complete", end='\r')
            if not nx.is_directed_acyclic_graph(graph):
                graph.remove_edge(node_from, node_to)
                edges_added -= 1

    # tqdm.write(f"{desc}: 100.00% complete")
    return list(graph.edges())


def create_tests(test_dir, test_sizes):
    if not os.path.exists(test_dir):
        os.makedirs(test_dir)

    output_queue = multiprocessing.Queue()

    processes = []
    # progress_bars = [tqdm(total=1, desc=f"Generating tests for {size}") for size in test_sizes]

    for size in test_sizes:
        process = multiprocessing.Process(target=generate_acyclic_graph_batch, args=(size, output_queue))
        processes.append(process)
        process.start()

    for _ in tqdm(range(len(processes)), desc="Overall Progress"):
        size, result = output_queue.get()
        with open(f'{test_dir}/{size}.in', 'w') as f:
            f.write(f'{size} {int(size * (size - 1) / 2)}\n')
            f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result))
        # progress_bars[test_sizes.index(size)].upd

    for process in processes:
        process.join()


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
    res = {'TwM': {'x': [], 'y': []}, 'TwL': {'x': [], 'y': []}, 'KwM': {'x': [], 'y': []}, 'KwL': {'x': [], 'y': []}}
    for file in os.listdir(results):
        algo, size = file.split('_')
        size = size.split('.')[0]
        f = open(results + '/' + file, 'r')
        res['TwM']['x'].append(int(size))
        res['TwL']['x'].append(int(size))
        res['KwM']['x'].append(int(size))
        res['KwL']['x'].append(int(size))
        for line in f:
            if 'Tarjan with matrix' in line:
                time = float(line.split()[-2])
                res['TwM']['y'].append(time)
            elif 'Tarjan with list' in line:
                time = float(line.split()[-2])
                res['TwL']['y'].append(time)
            elif 'Kahn with matrix' in line:
                time = float(line.split()[-2])
                res['KwM']['y'].append(time)
            elif 'Kahn with list' in line:
                time = float(line.split()[-2])
                res['KwL']['y'].append(time)
        f.close()
    return res


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
