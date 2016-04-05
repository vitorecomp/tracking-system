#!/bin/sh


#rodando a v sequencial
g++ -ggdb `pkg-config --cflags opencv` -o result *.cpp `pkg-config --libs opencv`;
