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
        
fps_values = [30, 60, 100, 200]
fps_labels = ["30 FPS", "60 FPS", "100 FPS", "200 FPS"]

fpsmeans = {fps: [] for fps in fps_values}

for dir in dir_paths:
    df = join_all_results_in_one(dir)[["Entities", "FPS"]].sort_values(by="FPS")
    for i, fps in enumerate(fps_values):
        fpsmeans[fps].append(np.interp(fps, df['FPS'], df["Entities"]))



width = 1
x = np.arange(len(dir_names))

for i, fps in enumerate(fps_values):
    fig, ax = plt.subplots(figsize=(8, 6))
    mult = 0
    for j, (att, measurement) in enumerate(dir_names.items()):
        rects = ax.bar(x[j], fpsmeans[fps][j], width, label=measurement)
        ax.bar_label(rects, padding=3, fmt="%.0f")
    
    ax.set_title(f'Entity count at {fps_labels[i]}')
    ax.set_ylabel('Entities')
    #ax.set_xticks(x, dir_names.values(), rotation=45, ha='right')
    ax.get_xaxis().set_visible(False)
    ax.set_yscale('log')
    ax.grid(axis='y', linestyle='--', alpha=0.7)
    ax.legend(loc='upper right')

plt.tight_layout()
plt.show()