import numpy as np
import matplotlib.pyplot as plt

file = open("../SupersonicFlowGpu/Pressure.txt")

x, y = file.readline().split(" ")[:-1]
print (x, y)

Z = np.zeros((x, y))