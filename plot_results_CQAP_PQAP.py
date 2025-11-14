import glob
import os

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd


def configure_matplotlib() -> None:
    plt.rcParams.update({"font.size": 18})


def load_and_process_data() -> pd.DataFrame:
    columns_to_keep = ["rl_gap", "sa_gap", "bb_gap", "gurobi_gap"]

    results = [
        r for r in glob.glob("results/*.csv") if ("test" not in r and "QAPLIB" not in r)
    ]

    dfs = []
    for file in results:
        df = pd.read_csv(file, usecols=columns_to_keep)
        dataset = os.path.splitext(os.path.basename(file))[0]
        generator, size = dataset.split("_")
        df["generator"] = generator if generator == "cqap" else "palubeckis"
        df["size"] = int(size)
        dfs.append(df)

    return pd.concat(dfs, ignore_index=True)


def plot_algorithm_performance(result_df: pd.DataFrame) -> None:
    algo_name_mapping = {"rl": "CLP", "sa": "SA", "bb": "B&B", "gurobi": "Gurobi"}
    problem_types = ["cqap", "palubeckis"]
    colors = plt.get_cmap("tab10")(np.linspace(0, 1, 10))[:5]

    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))
    axes = {"cqap": ax1, "palubeckis": ax2}

    for problem_type in problem_types:
        ax = axes[problem_type]
        numeric_df = result_df[result_df["generator"] == problem_type].select_dtypes(
            include=[np.number]
        )

        quantile_10 = numeric_df.groupby("size").quantile(0.1)
        quantile_50 = numeric_df.groupby("size").quantile(0.5)
        quantile_90 = numeric_df.groupby("size").quantile(0.9)

        for algo_idx, algo in enumerate(algo_name_mapping.keys()):
            sizes = quantile_50.index.tolist()
            quantile_10_values = quantile_10[f"{algo}_gap"].tolist()
            quantile_50_values = quantile_50[f"{algo}_gap"].tolist()
            quantile_90_values = quantile_90[f"{algo}_gap"].tolist()

            line_color = "yellow" if algo == "sa" else colors[algo_idx]
            line_style = "--" if algo == "sa" else "-"
            z_order = 3 if algo == "sa" else 1

            display_name = algo_name_mapping.get(algo, algo)

            ax.fill_between(
                sizes,
                quantile_10_values,
                quantile_90_values,
                alpha=0.3,
                color=line_color,
                label=f"{display_name} (10-90%)",
                zorder=z_order - 1,
            )

            ax.plot(
                sizes,
                quantile_50_values,
                label=f"{display_name} Median",
                color=line_color,
                linestyle=line_style,
                zorder=z_order,
            )

        ax.set_xlabel("Size")
        ax.set_ylabel("Gap")
        title_name = "CQAP" if problem_type == "cqap" else problem_type.capitalize()
        ax.set_title(f"{title_name} Gap vs. Size")
        ax.axhline(y=0, color="black", linestyle="--", label="Global Minimum")
        ax.legend()
        ax.grid(True)

    plt.tight_layout()
    plt.savefig("images/Omni_results.png")


def main() -> None:
    configure_matplotlib()
    result_df = load_and_process_data()
    plot_algorithm_performance(result_df)


if __name__ == "__main__":
    main()
