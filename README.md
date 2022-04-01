# Analysis Module for Transverse Single-Spin Asymmetry

This module use three packages.
1.	[ROOT](https://root.cern)
2.	[scikit-learn](https://scikit-learn.org/stable/modules/gaussian_process.html)
3.	[RooUnfold](https://gitlab.cern.ch/RooUnfold/RooUnfold/-/tree/master/)

Instruction for installing these packages can be found in the given links.

## Data Creation

Monte-Carlo data was created using the [SimChainDev](https://github.com/E1039-Collaboration/e1039-analysis/tree/master/SimChainDev) module and `make_tree.cc` script was used to create asymmetry added data with simple tree structure.

## Analysis

`analysis.ipynb` notebook was used to analyze data.

## Debugging

`debug.ipynb` notebook was used for debuging the main module.