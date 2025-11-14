#!/bin/bash

# filepath: /Users/theobourdais/Library/CloudStorage/OneDrive-CaliforniaInstituteofTechnology/Caltech/Research/QAP/qap-hypergraph-rl/cpp/generators/palubeckis_gen/run_palubeckis_gen.sh

# Define the executable path
EXECUTABLE="./sources/palubeckis_gen"

# Define the input files
INPUT_FILES=(
    "./input_70.dat"
    "./input_20.dat"
    "./input_40.dat"
    "./input_10.dat"
    "./input_15.dat"
)

# Run the executable with each input file
for INPUT_FILE in "${INPUT_FILES[@]}"; do
    $EXECUTABLE $INPUT_FILE
done