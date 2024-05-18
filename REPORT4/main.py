import os
import subprocess
from matplotlib import pyplot as plt
import numpy as np
from generators import generate_undirected_hamiltonian_cyclic_graph
from generators import generate_hamiltonian_cyclic_digraph
from generators import generate_directed_euler_cyclic_graph
from generators import generate_undirected_euler_cyclic_graph
from generators import generate_acyclic_undirected_graph
from generators import generate_acyclic_directed_graph
from IPython.display import display


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
        for size in test_sizes:
            result_ara = generate_acyclic_directed_graph(size, saturation)
            with open(f'{test_dir}/{"DA"}/{saturation}_{size}.in', 'w') as f:
                f.write(f'{size} {int(size * (size - 1)/2)}\n')
                f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result_ara))
                f.close()

        for size in test_sizes:
            result_f = generate_hamiltonian_cyclic_digraph(size, saturation)
            with open(f'{test_dir}/{"DC"}/Floresa_{saturation}_{size}.in', 'w') as f:
                f.write(f'{size} {int(saturation / 100 * size * (size - 1))}\n')
                if result_f == "0 0":
                    f.write("0 0\n")
                else:
                    f.write(
                        '\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result_f))
                f.close()

            # directed Euler
            result_eul = generate_directed_euler_cyclic_graph(size, saturation)
            with open(f'{test_dir}/{"DC"}/Fleury_{saturation}_{size}.in', 'w') as f:
                f.write(f'{size} {int(saturation / 100 * size * (size - 1))}\n')
                f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result_eul))
                f.close()

        for size in test_sizes:
            result_a = generate_acyclic_undirected_graph(size, saturation)
            with open(f'{test_dir}/{"UDA"}/{saturation}_{size}.in', 'w') as f:
                f.write(f'{size} {int(size * (size - 1) / 2)}\n')
                f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result_a))
                f.close()

        for size in test_sizes:
            result = generate_undirected_hamiltonian_cyclic_graph(size, saturation)
            with open(f'{test_dir}/{"UDC"}/Floresa_{saturation}_{size}.in', 'w') as f:
                f.write(f'{size} {int(saturation / 100 * size * (size - 1) / 2)}\n')
                if result == "0 0":
                    f.write("0 0\n")
                else:
                    f.write('\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result))
                f.close()

            # undirected Euler
            result_ud_eul = generate_undirected_euler_cyclic_graph(size, saturation)
            with open(f'{test_dir}/{"UDC"}/Fleury_{saturation}_{size}.in', 'w') as f:
                f.write(f'{size} {int(saturation / 100 * size * (size - 1) / 2)}\n')
                f.write(
                    '\n'.join(str(edge).replace("(", "").replace(")", "").replace(",", "") for edge in result_ud_eul))
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
                    f_in_n1 = f'{test_dir}/DA/{saturation}_{ts}.in'
                    f_out_n1 = f'{result_dir}/DA/{algo}_{saturation}_{ts}.out'
                    f_in = open(f_in_n1, 'r')
                    f_out = open(f_out_n1, 'w')
                    command = [f'{bins}/{algo}']
                    subprocess.run(command, stdin=f_in, stdout=f_out)
                    if v:
                        print(command, f'in={f_in_n1}, out={f_out_n1}')
                    f_in.close()
                    f_out.close()

                    f_in_n2 = f'{test_dir}/DC/Floresa_{saturation}_{ts}.in'
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
                    f_in_n1 = f'{test_dir}/UDA/{saturation}_{ts}.in'
                    f_out_n1 = f'{result_dir}/UDA/{algo}_{saturation}_{ts}.out'
                    f_in = open(f_in_n1, 'r')
                    f_out = open(f_out_n1, 'w')
                    command = [f'{bins}/{algo}']
                    subprocess.run(command, stdin=f_in, stdout=f_out)
                    if v:
                        print(command, f'in={f_in_n1}, out={f_out_n1}')
                    f_in.close()
                    f_out.close()

                    f_in_n2 = f'{test_dir}/UDC/Floresa_{saturation}_{ts}.in'
                    f_out_n2 = f'{result_dir}/UDC/{algo}_{saturation}_{ts}.out'
                    f_in2 = open(f_in_n2, 'r')
                    f_out2 = open(f_out_n2, 'w')
                    command = [f'{bins}/{algo}']
                    subprocess.run(command, stdin=f_in2, stdout=f_out2)
                    if v:
                        print(command, f'in={f_in_n2}, out={f_out_n2}')
                    f_in2.close()
                    f_out2.close()

                if "Fleury-directed" in algo:
                    f_in_n1 = f'{test_dir}/DA/{saturation}_{ts}.in'
                    f_out_n1 = f'{result_dir}/DA/{algo}_{saturation}_{ts}.out'
                    f_in = open(f_in_n1, 'r')
                    f_out = open(f_out_n1, 'w')
                    command = [f'{bins}/{algo}']
                    subprocess.run(command, stdin=f_in, stdout=f_out)
                    if v:
                        print(command, f'in={f_in_n1}, out={f_out_n1}')
                    f_in.close()
                    f_out.close()

                    f_in_n2 = f'{test_dir}/DC/Fleury_{saturation}_{ts}.in'
                    f_out_n2 = f'{result_dir}/DC/{algo}_{saturation}_{ts}.out'
                    f_in2 = open(f_in_n2, 'r')
                    f_out2 = open(f_out_n2, 'w')
                    command = [f'{bins}/{algo}']
                    subprocess.run(command, stdin=f_in2, stdout=f_out2)
                    if v:
                        print(command, f'in={f_in_n2}, out={f_out_n2}')
                    f_in2.close()
                    f_out2.close()

                if "Fleury-undirected" in algo:
                    f_in_n1 = f'{test_dir}/UDA/{saturation}_{ts}.in'
                    f_out_n1 = f'{result_dir}/UDA/{algo}_{saturation}_{ts}.out'
                    f_in = open(f_in_n1, 'r')
                    f_out = open(f_out_n1, 'w')
                    command = [f'{bins}/{algo}']
                    subprocess.run(command, stdin=f_in, stdout=f_out)
                    if v:
                        print(command, f'in={f_in_n1}, out={f_out_n1}')
                    f_in.close()
                    f_out.close()

                    f_in_n2 = f'{test_dir}/UDC/Fleury_{saturation}_{ts}.in'
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
    global time
    res_DA = {'Robertsa-Floresa': {'x': [], 'y': [], 'z': []}, 'Fleury': {'x': [], 'y': [], 'z': []}}
    res_UDA = {'Robertsa-Floresa': {'x': [], 'y': [], 'z': []}, 'Fleury': {'x': [], 'y': [], 'z': []}}
    res_DC = {'Robertsa-Floresa': {'x': [], 'y': [], 'z': []}, 'Fleury': {'x': [], 'y': [], 'z': []}}
    res_UDC = {'Robertsa-Floresa': {'x': [], 'y': [], 'z': []}, 'Fleury': {'x': [], 'y': [], 'z': []}}
    for dir in os.listdir(results):
        for file in os.listdir(f"{results}/{dir}"):
            algo, saturation, size = file.split('_')
            size = size.split('.')[0]
            f = open(results + '/' + dir + '/' + file, 'r')
            time = 0
            for line in f:
                if "Time" in line:
                    time = float(line.split(' ')[1])
            if dir == "DA":
                # if size not in res_DA['Robertsa-Floresa']['x']:
                #     res_DA['Robertsa-Floresa']['x'].append(size)
                # if size not in res_DA['Fleury']['x']:
                #     res_DA['Fleury']['x'].append(size)
                if 'Robertsa-Floresa' in algo:
                    res_DA['Robertsa-Floresa']['x'].append(int(size))
                    res_DA['Robertsa-Floresa']['y'].append(time)
                    res_DA['Robertsa-Floresa']['z'].append(float(saturation))
                if 'Fleury' in algo:
                    res_DA['Fleury']['x'].append(int(size))
                    res_DA['Fleury']['y'].append(time)
                    res_DA['Fleury']['z'].append(float(saturation))

            if dir == "DC":
                # if size not in res_DC['Robertsa-Floresa']['x']:
                #     res_DC['Robertsa-Floresa']['x'].append(size)
                # if size not in res_DC['Fleury']['x']:
                #     res_DC['Fleury']['x'].append(size)
                if 'Robertsa-Floresa' in algo:
                    res_DC['Robertsa-Floresa']['x'].append(int(size))
                    res_DC['Robertsa-Floresa']['y'].append(time)
                    res_DC['Robertsa-Floresa']['z'].append(float(saturation))
                if 'Fleury' in algo:
                    res_DC['Fleury']['x'].append(int(size))
                    res_DC['Fleury']['y'].append(time)
                    res_DC['Fleury']['z'].append(float(saturation))

            if dir == "UDA":
                # if size not in res_UDA['Robertsa-Floresa']['x']:
                #     res_UDA['Robertsa-Floresa']['x'].append(size)
                # if size not in res_UDA['Fleury']['x']:
                #     res_UDA['Fleury']['x'].append(size)
                if 'Robertsa-Floresa' in algo:
                    res_UDA['Robertsa-Floresa']['x'].append(int(size))
                    res_UDA['Robertsa-Floresa']['y'].append(time)
                    res_UDA['Robertsa-Floresa']['z'].append(float(saturation))
                if 'Fleury' in algo:
                    res_UDA['Fleury']['x'].append(int(size))
                    res_UDA['Fleury']['y'].append(time)
                    res_UDA['Fleury']['z'].append(float(saturation))

            if dir == "UDC":
                # if size not in res_UDC['Robertsa-Floresa']['x']:
                #     res_UDC['Robertsa-Floresa']['x'].append(size)
                # if size not in res_UDC['Fleury']['x']:
                #     res_UDC['Fleury']['x'].append(size)
                if 'Robertsa-Floresa' in algo:
                    res_UDC['Robertsa-Floresa']['x'].append(int(size))
                    res_UDC['Robertsa-Floresa']['y'].append(time)
                    res_UDC['Robertsa-Floresa']['z'].append(float(saturation))
                if 'Fleury' in algo:
                    res_UDC['Fleury']['x'].append(int(size))
                    res_UDC['Fleury']['y'].append(time)
                    res_UDC['Fleury']['z'].append(float(saturation))
            f.close()

    for algorithm, values in res_DA.items():
        for key in values:
            values[key].sort()

    for algorithm, values in res_UDA.items():
        for key in values:
            values[key].sort()

    for algorithm, values in res_DC.items():
        for key in values:
            values[key].sort()

    for algorithm, values in res_UDC.items():
        for key in values:
            values[key].sort()

    return res_DA, res_UDA, res_DC, res_UDC


def plot_graf3D(res, title):
    for algorithm in res:
        fig = plt.figure(figsize=(6, 8))  # Установка размера фигуры
        ax = fig.add_subplot(projection='3d')

        x = np.array(res[algorithm]['x'])
        y = np.array(res[algorithm]['y'])
        z = np.array(res[algorithm]['z'])

        # Создание сетки значений для осей X и Z
        X_unique = np.unique(x)
        Z_unique = np.unique(z)
        X, Z = np.meshgrid(X_unique, Z_unique)
        Y = np.zeros_like(X, dtype=float)

        # Заполнение сетки значениями Y, преобразованными в логарифмическую шкалу
        for i in range(len(x)):
            xi = np.where(X_unique == x[i])[0][0]
            zi = np.where(Z_unique == z[i])[0][0]
            Y[zi, xi] = y[i]   # Y[zi, xi] = np.log10(y[i]) if y[i] > 0 else np.nan  # Использование логарифма base 10

        # Построение поверхности
        ax.plot_surface(X, Z, Y, cmap='viridis')

        ax.set_xlabel('Number of vertices (n)')
        ax.set_ylabel('Saturation (s)')
        # ax.set_zlabel('Log Time (log10(t))')

        plt.title(f"{title} {algorithm} Algorithm")
        # fig.subplots_adjust(left=0.1, right=0.9, top=0.9, bottom=0.1)
        plt.tight_layout()
        # ax.view_init(elev=30, azim=45)
        # display(fig)
        plt.show()

