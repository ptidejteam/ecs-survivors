import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt
import math
import pandas as pd
import os

def join_all_results_in_one(dir):
    files = []
    for file in os.listdir(dir):
        if ".txt" in file:
            files.append(pd.read_csv(dir + file, header=None ,names=["Frame", "Entities", "FPS", "Frame Time (us)"]))
    
    concatinated_files = pd.concat(files, axis=0)
    print(concatinated_files.groupby(["FPS"])["Entities"].mean())
    print(concatinated_files.head())
    
    

dir_paths = [
    ".\\cmake-build-release\\app\\results\\baseline-no-physics\\",
    ".\\cmake-build-release\\app\\results\\collision-relationship\\",
    ".\\cmake-build-release\\app\\results\\collision-entity\\",
    ".\\cmake-build-release\\app\\results\\record-list\\",
    ".\\cmake-build-release\\app\\results\\spatial-hash-rebuilding\\",
    ".\\cmake-build-release\\app\\results\\box2d\\"
]

join_all_results_in_one(dir_paths[0])




files = [
    genfromtxt(".\\cmake-build-release\\app\\results\\baseline-no-physics\\baseline-no-physics-0.txt", delimiter=',')[5:],
    genfromtxt(".\\cmake-build-release\\app\\results\\collision-relationship\\collision-relationship-0.txt", delimiter=',')[5:],
    genfromtxt(".\\cmake-build-release\\app\\results\\collision-entity\\collision-entity-0.txt", delimiter=',')[5:],
    genfromtxt(".\\cmake-build-release\\app\\results\\record-list\\record-list-0.txt", delimiter=',')[5:],
    genfromtxt(".\\cmake-build-release\\app\\results\\spatial-hash-rebuilding\\spatial-hash-rebuilding-0.txt", delimiter=',')[5:],
    genfromtxt(".\\cmake-build-release\\app\\results\\box2d\\box2d-0.txt", delimiter=',')[5:]]

frames = []
for file in files:
    frames.append(file[:,0])


ys = []
xs = []
fig, ax = plt.subplots()

plt.xlim(right=6000)
plt.ylim(top=200, bottom=30)

for file in files:
    y=file.T[2]
    x=file.T[1]
    plt.plot(x,y)

plt.show()

