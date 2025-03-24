import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt
import math


files = [
    genfromtxt(".\\cmake-build-release\\app\\results\\baseline-no-physics\\baseline-no-physics-0.txt", delimiter=',')[5:],
    genfromtxt(".\\cmake-build-release\\app\\results\\collision-entity\\collision-entity-0.txt", delimiter=',')[5:],
    genfromtxt(".\\cmake-build-release\\app\\results\\collision-relationship\\collision-relationship-0.txt", delimiter=',')[5:],
    genfromtxt(".\\cmake-build-release\\app\\results\\spatial-hash-rebuilding\\spatial-hash-rebuilding-0.txt", delimiter=',')[5:],
    genfromtxt(".\\cmake-build-release\\app\\results\\box2d\\box2d-0.txt", delimiter=',')[5:]]

frames = []
for file in files:
    frames.append(file[:,0])


ys = []
xs = []
fig, ax = plt.subplots()

plt.xlim(right=15000)

for file in files:
    y=file.T[2]
    x=file.T[1]
    plt.plot(x,y)

plt.show()

