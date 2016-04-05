#!/bin/sh
#rodando a v sequencial
g++ -ggdb -std=c++0x -pthread `pkg-config --cflags opencv` -o tracker *.cpp `pkg-config --libs opencv` -Ofast;
