#!/bin/python
import pandas as pd
import matplotlib.pyplot as plt
filename = "./output/phi"
file = [filename + str(i) + ".out" for i in range(0,1000)]
for i in range(0,1000):
    print("plotting", file[i], "...")
    data = pd.read_csv(file[i])
    phi=list(data.phi)
    x=list(data.x)

    plt.plot(x,phi,"-")
    plt.title("Potential")

    plt.xlabel("x")
    plt.ylabel("phi")
    plt.savefig("./imagesOut/phi" + str(i) + ".png")
    plt.close()