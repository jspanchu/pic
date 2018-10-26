#!/bin/python
import pandas as pd
import matplotlib.pyplot as plt
filename = "./output/kinetic.out"
print("plotting", filename, "...")
data = pd.read_csv(filename)
k=list(data.k)
t=list(data.t)

plt.plot(t,k,"-")
plt.title("Kinetic Energy")

plt.xlabel("t")
plt.ylabel("KE")
plt.savefig("./imagesOut/kinetic.png")
plt.close()

