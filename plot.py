import pandas as pd
import matplotlib.pyplot as plt
data = pd.read_csv("phi.out")
phi=list(data.phi)
x=list(data.x)

plt.plot(x,phi,"-")
plt.title("Potential")

plt.xlabel("x")
plt.ylabel("phi")
plt.show()

