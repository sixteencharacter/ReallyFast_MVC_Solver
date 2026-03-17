#!/bin/sh

# $1 is the input path (/input/grid-6-7.txt)
# $2 is the output path (/output/grid-6-7.out)

# Create the output directory if it doesn't exist (just in case)
mkdir -p "$(dirname "$2")"
touch $2

# Run the program with redirection
# Using "$@" ensures all arguments are handled correctly
/app/runner < "$1" > "$2"