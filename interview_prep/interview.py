# a list of data that has a timestamp and a value -- tuple array
# 

import matplotlib.pyplot as plt
import numpy as np
import random
from datetime import datetime, timedelta
data = []
data2 = []

n = 100
time = datetime.now()


for i in range(n):
    data.append((time+timedelta(days=i), i + random.randint(-2,2)))
    data2.append((time+timedelta(days=i), n-i + random.randint(-2,2)))

# now, plot the data
xs = []
ys = []

ys2 = []
for i in range(n):
    xs.append(data[i][0])
    ys.append(data[i][1])
    ys2.append(data2[i][1])

fig, ax = plt.subplots()

# how to plot over this scatter
ax.scatter(xs, ys)
ax.scatter(xs, ys2, c="red")

timestamps = list(map(lambda x: x.timestamp(), xs))

coefficients = np.polyfit(timestamps, ys, 1)
slope = coefficients[0]
intercept = coefficients[1]

trend = np.poly1d(coefficients)
trend_y = trend(timestamps)

plt.plot(xs, trend_y)


plt.show()

# nvidia vs snp500