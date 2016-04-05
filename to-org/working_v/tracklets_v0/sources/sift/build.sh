#!/bin/sh

g++ -ggdb `pkg-config --cflags opencv` -o result *.cpp `pkg-config --libs opencv`;
