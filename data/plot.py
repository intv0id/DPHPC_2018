import os
import matplotlib.pyplot as plt
import numpy as np

for f1 in os.listdir("lsb"):
    for f2 in os.listdir("lsb/" + f1):

        folder = "lsb/" + f1 + "/" + f2
        files = os.listdir(folder)

        algo = f1
        graph = f2.split("_")[1]

        data = {}
        the_x = []
        the_y = []

        for name in files:
            print(name)
            f = open(folder + "/" + name, "r")
            l = f.readlines()
            if len(l) > 0:
                x = l[11].split(" ")
                n = int("".join(x[20:30]))
                y = float("".join(x[45:55]))
                if n not in data:
                    data[n] = []
                data[n].append(y)
                the_x.append(n)
                the_y.append(y)

        if len(the_x) > 0:

            base = np.median(data[1])
            unique_x = sorted(data.keys())
            unique_y = [np.median(data[x]) for x in unique_x]
            avg_y = [np.average(data[x]) for x in unique_x]

            speedup = [base / y for y in the_y]
            unique_speedup = [base / y for y in unique_y]
            avg_speedup = [base / y for y in avg_y]

            # Plot speedup
            plt.figure(figsize=(10, 8))
            plt.scatter(the_x, speedup)
            plt.plot(unique_x, unique_speedup, label="Median")
            plt.plot(unique_x, avg_speedup, label="Average")
            plt.title("Speedup " + algo + " " + graph)
            plt.legend()
            plt.ylim(0, 4)
            plt.savefig("images/speedup/speedup_" + algo + "_" + graph + ".png")

            # Plot speedup
            plt.figure(figsize=(10, 8))
            plt.scatter(the_x, the_y)
            plt.plot(unique_x, unique_y, label="Median")
            plt.plot(unique_x, avg_y, label="Average")
            plt.title("Time " + algo + " " + graph)
            plt.legend()
            plt.savefig("images/time/time_" + algo + "_" + graph + ".png")
