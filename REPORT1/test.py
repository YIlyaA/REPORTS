from matplotlib import pyplot as plt
import numpy as np

# x = np.array(
#     [90000, 1000, 100, 39000, 400, 59000, 700, 1300, 14000, 1600, 19000, 1900, 2200, 24000, 2500, 2800, 29000, 3100,
#      34000, 3400, 3700, 49000, 5000, 44000, 54000])
# y = np.array([
#     0.052, 0.0, 0.003, 0.02, 0.007, 0.031, 0.0, 0.0, 0.003, 0.0, 0.01, 0.0, 0.0, 0.01, 0.0, 0.0, 0.015, 0.0, 0.018, 0.0, 0.0, 0.027, 0.0, 0.021, 0.031])
# plt.plot(x, y)
# plt.show()


res = {'AVL': {'x': [10, 20, 30], 'y': ['0.000067472457886', '0.015527009963989', '1.955384731292725']}, 'BST': {'x': [10, 20, 30], 'y': ['0.000000953674316', '0.000002145767212', '0.000004053115845']}}
for algo in res:
    print(algo)


# Нарисовать только точки на графике
# times = utils.times_algo_from_dane(TIMES)
# for t in TYPE_STRINGS:
#     for algo, string in times:
#         if t == string:
#             x = times[algo, string]['x']
#             y = times[algo, string]['y']
#             plt.plot(x, y, '.', label=string + " gen, " + algo)
#
#     plt.xlabel("string")
#     plt.ylabel("seconds")
#     plt.legend(bbox_to_anchor=(1.02, 1), loc='upper left', borderaxespad=0)
#     plt.show()


# LineRegeression
# 1 ------------------------
# functions = {'ShellSort': lambda x: x*x, 'QuickSort': lambda x: x*x}
# other = lambda x: x*np.log(x)

# times, errors = utils.times_algo_from_dane(TIMES)
# print(errors)
# utils.plot_conf_s(TYPE_STRINGS, TEST_SIZES, times, CUTOFS, functions, other)

# 2-------------------------------
# functions = {'ShellSort': lambda x: x*x, 'QuickSort': lambda x: x*x}
# other = lambda x: x*np.log(x)

# times, errors = utils.times_algo_from_dane(TIMES)
# utils.plot_conf_alg(TEST_SIZES, times, CUTOFS, functions, other)


#############################################
# errors:
# data = {'x': x, 'y': y, 'er': errors}
# df = pd.DataFrame(data)
# print(df)

# errors on the graf:
# df.plot(kind='bar', x='x', y='y', yerr='er', capsize=5)
# plt.xlabel('X Label')
# plt.ylabel('Y Label')
# plt.title('Data with Error Bars')
# plt.grid(True)