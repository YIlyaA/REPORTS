str = 'Time for dynamic: 3.6759e-05 seconds'
time_brute = 0
if "dynamic" in str:
    time_brute = float(str.split(' ')[3])

print(time_brute)
