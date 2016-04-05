#makefile

#normal flags
CC = g++
CFLAGS =  -w -std=c++0x -fmax-errors=100 `pkg-config --cflags opencv`
LIBS = -l sqlite3 -pthread `pkg-config --libs opencv` -O3
LIB_TEST = -lcppunit

#compile project
all: templ sift surf
	#Finalizado

clean:
	rm -r -f ./results/
	mkdir results
	rm -r -f ./release/
	mkdir release
	mkdir release/bin

#################################################################################
#sift
#################################################################################
sift: ./release/sequential_sift #./release/parallel_sift

parallel_sift_files := $(shell find ./sift/parallel_current/ -type f -name *.cpp)
./release/parallel_sift: $(parallel_sift_files)
	$(CC) ./sift/parallel_current/*.cpp $(CFLAGS) -o ./release/parallel_sift $(LIBS)

sequential_sift_files := $(shell find ./sift/sequential_current/ -type f -name *.cpp)
./release/sequential_sift: $(sequential_sift_files)
	$(CC) ./sift/sequential_current/*.cpp $(CFLAGS) -o ./release/sequential_sift $(LIBS)

#################################################################################
#sift
#################################################################################
surf: ./release/sequential_surf #./release/parallel_surf

parallel_surf_files := $(shell find ./surf/parallel_current/ -type f -name *.cpp)
./release/parallel_surf: $(parallel_surf_files)
	$(CC) ./surf/parallel_current/*.cpp $(CFLAGS) -o ./release/parallel_surf $(LIBS)

sequential_surf_files := $(shell find ./surf/sequential_current/ -type f -name *.cpp)
./release/sequential_surf: $(sequential_surf_files)
	$(CC) ./surf/sequential_current/*.cpp $(CFLAGS) -o ./release/sequential_surf $(LIBS)

#################################################################################
#sift
#################################################################################
templ: ./release/sequential_templateMatch #./release/parallel_templateMatch

parallel_templateMatch_files := $(shell find ./templateMatch/parallel_current/ -type f -name *.cpp)
./release/parallel_templateMatch: $(parallel_templateMatch_files)
	$(CC) ./templateMatch/parallel_current/*.cpp $(CFLAGS) -o ./release/parallel_templateMatch $(LIBS)

sequetial_templateMatch_files := $(shell find ./templateMatch/sequential_current/ -type f -name *.cpp)
./release/sequential_templateMatch: $(sequetial_templateMatch_files)
	$(CC) ./templateMatch/sequential_current/*.cpp $(CFLAGS) -o ./release/sequential_templateMatch $(LIBS)

#################################################################################
#sift
#################################################################################
optical: ./release/parallel_opticalFlow ./release/sequential_opticalFlow

parallel_opticalFlow_files := $(shell find ./opticalFlow/parallel_current/ -type f -name *.cpp)
./release/parallel_opticalFlow: $(parallel_opticalFlow_files)
	$(CC) ./opticalFlow/parallel_current/*.cpp $(CFLAGS) -o ./release/parallel_opticalFlow $(LIBS)

sequential_opticalFlow_files := $(shell find ./opticalFlow/sequential_current/ -type f -name *.cpp)
./release/sequential_opticalFlow: $(sequential_opticalFlow_files)
	$(CC) ./opticalFlow/parallel_current/*.cpp $(CFLAGS) -o ./release/sequential_opticalFlow $(LIBS)
