import matplotlib.pyplot as plt
import time
import math
import numpy as np



def calcPressure(timeElapsed):
    pressure = (1 + math.sin(timeElapsed*36* 3.14/180)) * 100
    return pressure
#pressure = math.sin((time.time() - flowStartTime)*36*100 * 180/3.14)))
dataPoints = list(np.arange(0.0, 60, 0.1))
x = range(60)
pressure = [calcPressure(i) for i in dataPoints]

plt.plot(dataPoints, pressure)
plt.ylabel('some numbers')
plt.show()