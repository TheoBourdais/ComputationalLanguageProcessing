# QAP Problem Generators

This directory contains two problem generators for the Quadratic Assignment Problem (QAP) used in our experimental evaluation. Both generators are direct implementations from their original papers.

## Generator Sources

| Generator | Original Paper | Language | Implementation |
|-----------|----------------|----------|----------------|
| **CQAP** | Drugan (2015) | Java | Copied from original repository |
| **PQAP** | Palubeckis (2000) | C | Implemented from paper description |

## CQAP Generator

**Source**: Drugan, M.M. (2015). Generating QAP instances with known optimum solution and additively decomposable cost function. Journal of Combinatorial Optimization, 30(4), 1138-1172.

**Original Repository**: <https://github.com/MadalinaDrugan/Quadratic-Assignment-problem-instance-generator>

Generates QAP instances with known optimal solutions using composite construction methods.

**Parameters used in our experiments**: See files `test_problems/generators/cqap/setInput_*.param` for configurations for sizes 20, 40, 60, and 80.

## PQAP Generator

**Source**: Palubeckis, G. (2000). An algorithm for construction of test cases for the quadratic assignment problem. Informatica, 11(3), 281-296.

Generates QAP instances using graph-based construction on grid layouts.

**Parameters Used in Our Experiments**: See files `test_problems/generators/pqap/input_*.dat` for configurations for sizes 15, 20, 40, and 70. Parameters format: `n_objects x_dim y_dim n_graphs size_lower size_upper weight_bound seed n_instances`

## Generated Test Sets

The generators were used to create test instances stored in:

- `test_cqap_*/`: CQAP instances for sizes 20, 40, 60, 80
- `test_pqap_*/`: PQAP instances for sizes 15, 20, 40, 70
- `QAPLIB/`: Standard benchmark instances

Each instance includes `.dat` problem files and `.sln` solution files with known optimal costs. Additionnally, QAPLIB include .npy files for D and F matrices.
