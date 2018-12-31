import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import  glob
import seaborn as sns
import re
from io import StringIO

sns.set_palette("husl")
sns.set_style("whitegrid")

def tocategory(col): return pd.Categorical(col)
def underscoretospace(x): return x.replace("_", " ") if type(x) is str else x
def get_file(filename):
    with open(filename) as f:
        return re.sub(" +", ' ', f.read())
def q5(x): return x.quantile(.05)
def q95(x): return x.quantile(.95)

def preprocess(df):
    df[["Algorithm", "Graph_name"]] = df[["Algorithm", "Graph_name"]].apply(tocategory)
    df.columns = df.columns.map(underscoretospace)
    df["time"] *= 10**6
    df['Graph'] = df.apply(lambda x: (x['Graph name'], x['Graph nodes']), 1)
    df["Throughput"] = 1/df["time"]
    df['speedup'] = df.apply(lambda x: df[(df.Algorithm == x.Algorithm) & (df.Graph == x.Graph) & (df['Max threads'] == 1)].time.median() / x.time, 1)
    return df

def get_df(dataset):
    filenames = glob.glob(f"data/{dataset}/*/lsb.*.r*") + glob.glob(f"data/{dataset}/lsb.*.r*")
    df = pd.concat(pd
      .read_csv(StringIO(get_file(filename)), sep=" ", comment="#", skipinitialspace=True, index_col="id")
      .applymap(underscoretospace)
      .dropna("columns", "all")
      for filename in filenames
    )
    return preprocess(df)

def make_summaries(df, dataset, variable):
    assert variable in ["time", "speedup"]
    s = df[["Algorithm", "Graph", "Max threads", "speedup", "time"]]\
        .groupby(("Graph", "Algorithm", "Max threads"))\
        .agg(["median", "std"])  
    s[variable].to_csv(f"summaries/{dataset}/{variable}_summary.csv")
    s[variable].to_latex(f"summaries/{dataset}/{variable}_summary.tex")

def plot(df, dataset, diff, x, y, hue, ylogscale=True):
    assert diff in ["Graph", "Algorithm"]
    for i, e in enumerate(df[diff].unique()):
        fig, ax = plt.subplots(figsize=(12, 7))
        sns.lineplot(
            ax=ax, 
            data=df[df[diff] == e], 
            x=x, 
            y=y, 
            hue=hue,
            ci="sd",
            err_style="bars",
        )
        if ylogscale: ax.set(yscale="log")
        ax.set_xticks(2**np.arange(0, 6, 1))
        if diff == "Graph": ax.set_title(f"{e[0]} - {e[1]} nodes")
        else: ax.set_title(f"{e}")
        ax.set_ylim(0,)
        #fig.savefig(f"../presentations/final/graphics/BoostKruskalSpeedup{i}.png", dpi=450)
        plt.show()
