import numpy as np
import matplotlib.pyplot as plt
import sqlite3

from mpl_toolkits.mplot3d import Axes3D

# def randrange(n, vmin, vmax):
#     return (vmax - vmin)*np.random.rand(n) + vmin
def load_data():
    conn = sqlite3.connect('winstats.sqlite')
    cursor = conn.cursor()
    results = cursor.execute("select num_offense, num_defense, win_offense*1.0/total_armies*100.0 percent_offense from winstats")
    xs = []
    ys = []
    zs = []
    for row in results:
        xs.append(row[0])
        ys.append(row[1])
        zs.append(row[2])
    return xs, ys, zs

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
n = 100
for c, m, zl, zh in [('r', 'o', -50, -25), ('b', '^', -30, -5)]:
    xs, ys, zs = load_data()
    ax.scatter(xs, ys, zs, c=c, marker=m)

ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

plt.show()
