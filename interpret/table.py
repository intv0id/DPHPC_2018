import numpy as np
from tabulate import tabulate

f = open('data.csv', 'r')

data = {}

for line in f.readlines():
    algo, p, graph_name, graph_node, value = line.split(",")

    p = int(p)
    if p > 1:
        graph = graph_name + " -> " + graph_node
        if graph not in data:
            data[graph] = {}
        if algo not in data[graph]:
            data[graph][algo] = []
        data[graph][algo].append(max(min(float(value), 1), 0))

algos = list(data[list(data.keys())[0]].keys())

def convert(x):
    s = str(int(x * 100))
    return s

values = []
for graph in data:
    print(" / ".join(algos))
    print("Graph", graph)
    line = [graph]
    for algo in algos:
        if algo in data[graph]:
            l = data[graph][algo]
            line.append(convert(max(l)) + " (" + convert(np.median(l)) + ")")
        else:
            line.append("-")
    values.append(line)

print(tabulate(values, headers=["Graph"] + algos, tablefmt="latex", stralign='right'))
