#!/usr/bin/env bash

echo "This script depends on raptor2 and graphviz";
echo "Usage: ./plotOWL.sh <INFILE>";
echo "The resulting image will be saved in the directory of <INFILE>";

rapper -o dot $1 | dot -Tpng -o $1.png
