############################################################################################################
#makefile
#
#
#
#
#
############################################################################################################
#Static variables
############################################################################################################
#compiler
CC = g++

#######################################
#flags
CFLAGS = -std=c++11 `pkg-config --cflags opencv`

#######################################
#libs
LIBS = -pthread -fopenmp  `pkg-config --libs opencv` -O4

#######################################
#compile project
#######################################
all: ./bin/main.o ./bin/tracker.o liberies
	$(CC) ./bin/*.o $(CFLAGS) -o ./release/database_gen $(LIBS)

#######################################
#compile project
#######################################
clean:
	rm -r -f ./bin/
	mkdir bin
	rm -r -f ./release/
	mkdir release

############################################################################################################
#Main
############################################################################################################
./bin/main.o: ./source/main.cpp
	$(CC) -c ./source/main.cpp  $(CFLAGS) -o ./bin/main.o

############################################################################################################
#ManangerCore
############################################################################################################

############################################################################################################
#Tracklet
############################################################################################################
./bin/tracker.o: ./source/src/Tracker.cpp ./source/header/Tracker.hpp
	$(CC) -c ./source/src/Tracker.cpp $(CFLAGS) -o ./bin/tracker.o

############################################################################################################
#Liberys
############################################################################################################
############################################################################################################
#Libs compilations
############################################################################################################
liberies: ./bin/json.o output
./bin/json.o: ./source/libs/jsonParser/json.h ./source/libs/jsonParser/jsoncpp.cpp
	$(CC) -c ./source/libs/jsonParser/jsoncpp.cpp $(CFLAGS) -o ./bin/json.o

############################################################################################################
#output - compilation
############################################################################################################
output: bin/IOMananger.o bin/IOManangerL.o bin/IOBasetypes.o bin/IOEntities.o

bin/IOMananger.o: ./source/libs/output_s/IOMananger.cpp ./source/libs/output/IOMananger.hpp
	$(CC) -c ./source/libs/output_s/IOMananger.cpp $(CFLAGS) -o ./bin/IOMananger.o

bin/IOManangerL.o: ./source/libs/output_s/IOMananger_Local.cpp ./source/libs/output/IOMananger.hpp
	$(CC) -c ./source/libs/output_s/IOMananger_Local.cpp $(CFLAGS) -o ./bin/IOManangerL.o

bin/IOBasetypes.o: ./source/libs/output_s/BaseTypes.cpp ./source/libs/output//BaseTypes.hpp
	$(CC) -c ./source/libs/output_s/BaseTypes.cpp $(CFLAGS) -o ./bin/IOBasetypes.o

bin/IOEntities.o: ./source/libs/output_s/Entities.cpp ./source/libs/output/Entities.hpp
	$(CC) -c ./source/libs/output_s/Entities.cpp $(CFLAGS) -o ./bin/IOEntities.o
