CC=g++
CFLAGS=-std=c++11

all: OWLConverter

OWLConverter: OWLConverter.cpp
	g++ OWLConverter.cpp $(CFLAGS) -o OWLConverter
