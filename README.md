# Discovering Algorithms with Computational Language Processing

Official repository for the paper: **[Discovering Algorithms with Computational Language Processing](https://arxiv.org/abs/2507.03190)**

This repository contains the complete experimental setup, datasets, and results for evaluating Computational Language Processing (CLP) against traditional optimization algorithms on the Quadratic Assignment Problem (QAP).

## Table of Contents

- [Discovering Algorithms with Computational Language Processing](#discovering-algorithms-with-computational-language-processing)
  - [Table of Contents](#table-of-contents)
  - [Overview](#overview)
  - [Repository Structure](#repository-structure)
  - [Datasets](#datasets)
    - [Synthetic Problem Generators](#synthetic-problem-generators)
    - [Benchmark Dataset](#benchmark-dataset)
  - [Algorithms Compared](#algorithms-compared)
    - [Performance Metrics](#performance-metrics)
  - [Experimental Results](#experimental-results)
    - [Key Findings](#key-findings)
    - [Result Files](#result-files)
  - [Installation \& Usage](#installation--usage)
    - [Prerequisites](#prerequisites)
    - [Generate Plots](#generate-plots)
    - [Problem Generators](#problem-generators)
  - [Citation](#citation)

## Overview

This research introduces Computational Language Processing (CLP), a novel approach to algorithm generation by treating algorithms as sequences of actions that can be learned and generated through reinforcement learning. We evaluate CLP's performance on the Quadratic Assignment Problem (QAP), comparing it against established optimization methods.

## Repository Structure

```code
ComputationalLanguageProcessing/
├── README.md                      # This file
├── results/                       # Experimental results
│   ├── cqap_*.csv                # CQAP generator results (sizes 20, 40, 60, 80)
│   ├── pqap_*.csv                # PQAP generator results (sizes 15, 20, 40, 70)
│   ├── QAPLIB.csv                # QAPLIB benchmark results
│   └── QAPLIB_summary.csv        # Processed QAPLIB summary
├── test_problems/                 # Problem instances and generators
│   ├── generators/               # Problem generators
│   │   ├── cqap/                # CQAP generator (Drugan)
│   │   └── pqap/                # PQAP generator (Palubeckis)
│   ├── QAPLIB/                  # QAPLIB benchmark instances
│   └── test_*/                  # Generated test instances
├── images/                       # Generated plots and figures
│   └── Omni_results.png         # Algorithm performance comparison
├── plot_results_CQAP_PQAP.py    # Plotting script for synthetic datasets
└── plot_results_QAPLIB.py       # Plotting script for QAPLIB results
```

## Datasets

### Synthetic Problem Generators

1. **CQAP Generator** ([Drugan, 2015](https://dl.acm.org/doi/10.1007/s10878-013-9689-6))
   - Generates QAP instances with known optimal solutions
   - Additively decomposable cost functions
   - Test sizes: 20, 40, 60, 80 facilities

2. **PQAP Generator** ([Palubeckis, 2000](https://link.springer.com/article/10.1023/A:1008303023616))
   - Graph-based QAP instance generation
   - Realistic problem structure
   - Test sizes: 15, 20, 40, 70 facilities

### Benchmark Dataset

- **QAPLIB** ([Burkard et al., 1997](https://coral.ise.lehigh.edu/data-sets/qaplib/))
  - Standard benchmark library for QAP instances
  - Includes classic problems: bur26*, chr12-25*, els19, etc.
  - Real-world and synthetic instances with known optimal solutions or best-known solutions.

## Algorithms Compared

| Algorithm | Description |
|-----------|-------------|
| **CLP** | Computational Language Processing - our proposed method using reinforcement learning with sequence modeling |
| **SA** | Simulated Annealing - classical metaheuristic optimization |
| **B&B** | Branch and Bound - exact optimization method |
| **Gurobi** | Commercial optimization solver |

### Performance Metrics

- **Gap**: Relative deviation from known optimal solution
- **Time**: Computational time to solution
- **Success Rate**: Percentage of instances solved to optimality

## Experimental Results

### Key Findings

1. **Optimal Performance on Synthetic Data**: CLP achieves 0% gap (optimal solutions) on all CQAP tested
2. **Competitive QAPLIB Performance**: Strong results on standard benchmarks
3. **Scalability**: Consistent performance across different problem sizes
4. **Robustness**: Reliable performance across diverse problem structures

### Result Files

- `results/cqap_*.csv` and `results/pqap_*.csv`: Contains instance name, optimal cost, solution permutations, and performance gaps for each algorithm
- `results/QAPLIB.csv`: QAPLIB benchmark results with optimality ratios
- Generated visualizations show algorithm performance distributions across problem sizes

## Installation & Usage

### Prerequisites

```bash
pip install pandas matplotlib numpy
```

### Generate Plots

```bash
# Plot synthetic dataset results
python plot_results_CQAP_PQAP.py

# Plot QAPLIB benchmark results  
python plot_results_QAPLIB.py
```

### Problem Generators

The repository includes the original problem generators:

- **CQAP Generator**: Java implementation in `test_problems/generators/cqap/`
- **PQAP Generator**: C implementation in `test_problems/generators/pqap/`

See individual README files in generator directories for compilation and usage instructions.

## Citation

If you use this code or data in your research, please cite:

```bibtex
@article{bourdais2025discovering,
  title={Discovering algorithms with computational language processing},
  author={Bourdais, Theo and Gnanasekaran, Abeynaya and Owhadi, Houman and Sahai, Tuhin},
  journal={arXiv preprint arXiv:2507.03190},
  year={2025}
}
```
