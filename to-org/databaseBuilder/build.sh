#!/bin/sh


#rodando a v sequencial
g++ -ggdb -pthread `pkg-config --cflags opencv` -o result ./main.cpp `pkg-config --libs opencv` -O4;
g++ -ggdb -pthread `pkg-config --cflags opencv` -o video ./image_video.cpp `pkg-config --libs opencv` -O4;
