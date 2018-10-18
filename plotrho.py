#!/bin/python
import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv("rho.out")
rho=list(data.rho)
x=list(data.x)

plt.plot(x,rho,"-")
plt.title("Desnity")

plt.xlabel("x")
plt.ylabel("Density")
plt.show()

