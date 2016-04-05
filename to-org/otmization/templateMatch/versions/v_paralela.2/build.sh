#!/bin/sh


#rodando a v sequencial
g++ -ggdb -pthread `pkg-config --cflags opencv` -o result *.cpp `pkg-config --libs opencv` -O4;
