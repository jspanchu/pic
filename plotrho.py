#!/bin/python
import os
import pandas as pd
import matplotlib.pyplot as plt
filename = "./output/rho"
file = [filename + str(i) + ".out" for i in range(0,1000)]
for i in range(0,1000):
    print("plotting", file[i], "...")
    data = pd.read_csv(file[i])
    rho=list(data.rho)
    x=list(data.x)

    plt.plot(x,rho,linewidth = 2, linestyle = "--")
    plt.title("Density")

    plt.xlabel("x")
    plt.ylabel("rho")
    plt.savefig("./imagesOut/rho" + str(i) + ".png")
    plt.close()

os.system("cd ./imagesOut/ && convert -delay 10 -loop 1 *.png Animation.gif")


