import numpy as np
import matplotlib.pyplot as plt
import mpl_toolkits
from math import sin, pi, cos
from mpl_toolkits.mplot3d import Axes3D

def rastrigin(x, y):
    return 20 + (x * x - 10 * cos(2 * pi * x)) + (y * y - 10 * cos(2 * pi * y))

def michalewicz(x, y):
    return -(sin(x) * sin(x * x / pi) ** 20 + sin(y) * sin(2 * y * y / pi) ** 20)

def shubert(x, y):
    return sum(i * cos((i + 1) * x + i) for i in range(1, 6)) * sum(i * cos((i + 1) * y + i) for i in range(1, 6))

def sphere(x, y):
    return x * x + y * y    


x, y = np.linspace(-1, 1, 500), np.linspace(-1, 1, 500)
X, Y = np.meshgrid(x, y)
Z = np.array([np.array([rastrigin(x, y) for x, y in zip(xLine, yLine)]) for xLine, yLine in zip(X, Y) ])
fig = plt.figure(figsize=(8, 6))
ax = plt.axes(projection='3d')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1,cmap='jet', edgecolor = 'none')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel("Rastrigin's function(x, y)")
plt.title("Rastrigin's Function")
plt.show()
Z = np.array([np.array([shubert(x, y) for x, y in zip(xLine, yLine)]) for xLine, yLine in zip(X, Y) ])
fig = plt.figure(figsize=(8, 6))
ax = plt.axes(projection='3d')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1,cmap='jet', edgecolor = 'none')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel("Shubert's function(x, y)")
plt.title("Shubert's Function")
plt.show()
Z = np.array([np.array([sphere(x, y) for x, y in zip(xLine, yLine)]) for xLine, yLine in zip(X, Y) ])
fig = plt.figure(figsize=(8, 6))
ax = plt.axes(projection='3d')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1,cmap='jet', edgecolor = 'none')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel("Sphere function(x, y)")
plt.title("Sphere Function")
plt.show()
x, y = np.linspace(1, 3, 500), np.linspace(1, 3, 500)
X, Y = np.meshgrid(x, y)
Z = np.array([np.array([michalewicz(x, y) for x, y in zip(xLine, yLine)]) for xLine, yLine in zip(X, Y) ])
fig = plt.figure(figsize=(8, 6))
ax = plt.axes(projection='3d')
ax.plot_surface(X, Y, Z, rstride=1, cstride=1,cmap='jet', edgecolor = 'none')
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel("Michalewicz's function(x, y)")
plt.title("Michalewicz's Function")
plt.show()

