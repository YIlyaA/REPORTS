import random

def generate_knapsack_data(n):
    max_size = random.randint(5, 50)
    max_value = random.randint(10, 70)

    items = []

    for _ in range(n):
        size = random.randint(1, max_size)
        value = random.randint(1, max_value)
        items.append((size, value))

    return items


# # # Parametry
# n = 20  # liczba przedmiotów
# max_capacity = 30
#
# # Generowanie danych
# items = generate_knapsack_data_const_capacity(n)
#
# # Wyświetlanie danych w formacie
# print(n, max_capacity)
# # print(items)
# for size, value in items:
#     print(size, value)
