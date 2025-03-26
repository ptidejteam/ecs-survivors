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
    
    return pd.concat(files, axis=0).groupby(["Frame"]).mean()
    # concatinated_files[["Entities", "FPS"]].plot(x = "Entities", y = "FPS", logx=True)
    # print(concatinated_files.groupby(["FPS"])["Entities"].mean())
    # print(concatinated_files.head())
    


dir_paths = [
    ".\\cmake-build-release\\app\\results\\baseline-no-physics\\",
    ".\\cmake-build-release\\app\\results\\box2d\\",
    ".\\cmake-build-release\\app\\results\\spatial-hash-rebuilding\\",
    ".\\cmake-build-release\\app\\results\\record-list\\",
    ".\\cmake-build-release\\app\\results\\collision-entity\\",
    ".\\cmake-build-release\\app\\results\\collision-relationship\\"
]

dir_names = {
    ".\\cmake-build-release\\app\\results\\baseline-no-physics\\" : "baseline-no-physics",
    ".\\cmake-build-release\\app\\results\\box2d\\"  : "box2d",
    ".\\cmake-build-release\\app\\results\\spatial-hash-rebuilding\\" : "spatial-hash-rebuilding",
    ".\\cmake-build-release\\app\\results\\record-list\\": "record-list",
    ".\\cmake-build-release\\app\\results\\collision-entity\\": "collision-entity",
    ".\\cmake-build-release\\app\\results\\collision-relationship\\": "collision-relationship"
}

ax_line = None
for dir in dir_paths:
    df = join_all_results_in_one(dir)[["Entities", "FPS"]]
    if ax_line is None:
        ax_line = df.plot.area(figsize=(10, 5),  ylabel="FPS", ylim=(30,200), x = "Entities", y = "FPS", logx=True, label=dir_names[dir], stacked=False)
    else:
        df.plot.area(ax=ax_line,ylim=(30,200), x = "Entities", y = "FPS", logx=True, label=dir_names[dir], stacked=False)
        
ax_bar = None
fpsmeans = {
    "baseline-no-physics": [],
    "box2d": [],
    "spatial-hash-rebuilding" : [],
    "record-list": [],
    "collision-entity": [],
    "collision-relationship": []
    
}
for dir in dir_paths:
    df = join_all_results_in_one(dir)[["Entities", "FPS"]].sort_values(by="FPS")    
    fpsmeans[dir_names[dir]].append(np.interp(30, df['FPS'], df["Entities"]))
    fpsmeans[dir_names[dir]].append(np.interp(60, df['FPS'], df["Entities"]))
    fpsmeans[dir_names[dir]].append(np.interp(100, df['FPS'], df["Entities"]))
    fpsmeans[dir_names[dir]].append(np.interp(200, df['FPS'], df["Entities"]))   

fpss = [ "30 FPS", "60 FPS","100 FPS","200 FPS"]

fig, ax = plt.subplots(layout='constrained', figsize=(24, 6))

x = np.arange(len(fpss))
width = 0.16
mult = 0

for att, measurement in fpsmeans.items():
    offset = width * mult
    rects = ax.bar(x + offset, measurement, width, label=att)
    ax.bar_label(rects, padding = 3, fmt="%.0f")
    mult += 1

ax.set_ylabel('Entities')
ax.set_title('Entity count at common FPS values')
ax.set_xticks(x + width, fpss)
ax.legend(loc='upper right')



plt.yscale('log')
plt.grid(axis='y', linestyle='--', alpha=0.7)
plt.tight_layout()
    
plt.show()