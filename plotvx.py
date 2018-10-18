#!/bin/python
import os
import pandas as pd
import matplotlib.pyplot as plt
filename = "./output/fvx"
file = [filename + str(i) + ".out" for i in range(1,1001)]
for i in range(1,1001):
    print("plotting", file[i], "...")
    data = pd.read_csv(file[i])
    v=list(data.v)
    x=list(data.x)

    plt.scatter (x, v, s = 1, c = 'k', marker = 'o')
    plt.title("Phase Trajectories")

    plt.xlabel("x")
    plt.ylabel("v")
    plt.savefig("./imagesOut/fvx" + str(i) + ".png")
    plt.close()

os.system("convert -delay 10 -loop 1 *.png Animation.gif")


