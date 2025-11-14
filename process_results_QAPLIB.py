import os
import re

import matplotlib.pyplot as plt
import pandas as pd


def configure_matplotlib() -> None:
    plt.rcParams.update({"font.size": 16})


def extract_problem_details(problem_id: str) -> pd.Series:
    """Extracts size, dataset name, and problem name from the problem_id."""
    problem_name = os.path.splitext(os.path.basename(problem_id))[0]
    match = re.match(r"([a-zA-Z]+)(\d+)", problem_name)

    if not match:
        raise ValueError(
            f"Filename '{problem_name}' does not match expected format 'datasetname_size'."
        )

    dataset_name = match.group(1)
    size = int(match.group(2))

    return pd.Series(
        {"dataset_name": dataset_name, "size": size, "problem_name": problem_name}
    )


def find_best_model(row: pd.Series) -> str:
    """Returns the best model for a given row based on the minimum gap."""
    pretty_names = {"rl": "CLP", "sa": "SA", "gurobi": "Gurobi"}

    gap_columns = [col for col in row.index if col.endswith("_gap")]
    min_gap = row[gap_columns].min()

    best_models = [col[:-4] for col in gap_columns if row[col] == min_gap]

    pretty_best_models: list[str] = []
    for model in best_models:
        if model is not None:
            pretty_name = pretty_names.get(model, model)
            if pretty_name is not None:
                pretty_best_models.append(pretty_name)

    return "/".join(pretty_best_models)


def add_optimality_metrics(df: pd.DataFrame) -> None:
    """Add optimality ratio columns and percentage strings for each gap column."""
    gap_columns = [col for col in df.columns if col.endswith("_gap")]

    for col in gap_columns:
        base_col = col[:-4]
        # Add optimality ratio
        df[f"{base_col}_optimality_ratio"] = df[col] / df["optimal_cost"]
        # Add percentage string
        df[f"{base_col}_optimality_ratio_percent"] = df[
            f"{base_col}_optimality_ratio"
        ].apply(lambda x: f"{x * 100:.1f}")


def create_summary_table(df: pd.DataFrame) -> pd.DataFrame:
    """Create a wide-format summary table by splitting data into two halves."""
    ratio_columns = [col for col in df.columns if col.endswith("_optimality_ratio")]
    columns_to_keep = ["problem_name", "best_model"] + [
        f"{col}_percent" for col in ratio_columns
    ]

    df_summary = df[columns_to_keep].copy()
    n_rows = len(df_summary)
    mid_point = (n_rows + 1) // 2

    first_half = df_summary.iloc[:mid_point].reset_index(drop=True)
    second_half = df_summary.iloc[mid_point:].reset_index(drop=True)

    # Rename second half columns to avoid conflicts
    second_half_renamed = second_half.rename(
        columns={col: f"{col}_2" for col in second_half.columns}
    )

    return pd.concat([first_half, second_half_renamed], axis=1)


def load_and_process_qaplib_data() -> pd.DataFrame:
    """Load and process QAPLIB results data."""
    qaplib_results = pd.read_csv("results/QAPLIB.csv")

    # Extract problem details
    problem_details = qaplib_results["instance"].apply(extract_problem_details)
    qaplib_results = pd.concat([qaplib_results, problem_details], axis=1)

    # Sort by dataset name, size (descending), and problem name
    qaplib_results = qaplib_results.sort_values(
        by=["dataset_name", "size", "problem_name"], ascending=[True, False, True]
    )

    # Add best model information
    qaplib_results["best_model"] = qaplib_results.apply(find_best_model, axis=1)

    # Add optimality ratios and percentage columns
    add_optimality_metrics(qaplib_results)

    return qaplib_results


def main() -> None:
    configure_matplotlib()
    qaplib_results = load_and_process_qaplib_data()
    summary_table = create_summary_table(qaplib_results)
    summary_table.to_csv("results/QAPLIB_summary.csv", index=False)


if __name__ == "__main__":
    main()
