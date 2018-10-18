import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
data = pd.read_csv("fvx.out")
fig = plt.figure()
ax = Axes3D(fig)
f=list(data.f)
v=list(data.v)
x=list(data.x)

ax.plot(x,v,f,".")
plt.title("Velocity Distribution")

plt.xlabel("x")
plt.ylabel("v")
plt.savefig("fvx.png")
plt.show()

