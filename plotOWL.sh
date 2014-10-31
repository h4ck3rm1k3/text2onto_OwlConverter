#!/usr/bin/env bash

if [ -z "$1" ] ; then
    echo "Error: No Input argument supplied!";
    echo "Usage: ./plotOWL.sh <OWL_FILE>";
    echo "Where OWL_FILE is an RDF formatted ontology file.";
    exit -1;
fi

if [ ! -f $1 ] ; then
    echo "Error: Invalid input file! Needs to be RDF formatted ontology file.";
    exit -2;
elif ! type -P rapper 2>/dev/null; then
    echo "Please install raptor2!";
    exit -3;
elif ! type -P dot 2>/dev/null; then
    echo "Please install GraphViz!";
    exit -4;
fi

rapper -o dot $1 | dot -Tpng -o $1.png

echo "Plot was saved as $1.png";

