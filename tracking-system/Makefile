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
LIBS = -pthread -O4

#######################################
#compile project
#######################################
all: bin/main.o
	rm -f log.txt
	$(CC) ./bin/*.o $(CFLAGS) -o ./release/tracking_system $(LIBS) `pkg-config --libs opencv`

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
bin/main.o: ./source/main.cpp mananger_core liberies
	$(CC) -c ./source/main.cpp $(CFLAGS) -o ./bin/main.o $(LIBS)

mananger_core: mananger tracklet


############################################################################################################
#ManangerCore
############################################################################################################
mananger: bin/ManangerCore.o

bin/ManangerCore.o: ./source/src/ManangerCore.cpp ./source/header/ManangerCore.hpp
	$(CC) -c ./source/src/ManangerCore.cpp $(CFLAGS) -o ./bin/ManangerCore.o $(LIBS)

############################################################################################################
#Tracklet
############################################################################################################
tracklet: bin/Tracklet.o bin/Queue.o

bin/Tracklet.o: ./source/src/Tracklet.cpp ./source/header/Tracklet.hpp
	$(CC) -c ./source/src/Tracklet.cpp $(CFLAGS) -o ./bin/Tracklet.o $(LIBS)

bin/Queue.o: ./source/src/queue/queue.cpp ./source/header/queue/queue.hpp
	$(CC) -c ./source/src/queue/queue.cpp $(CFLAGS) -o ./bin/Queue.o $(LIBS)

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

##############################################################
#Testes de execução autmatica
##############################################################
#test libs
LIB_TEST = -lcppunit

database_gen: ./bin/main_gen.o ./bin/tracker.o liberies
	$(CC) ./bin/*.o $(CFLAGS) -o ./release/database_gen $(LIBS)

./bin/main_gen.o: ./database_gen/main.cpp
	$(CC) -c ./database_gen/main.cpp  $(CFLAGS) -o ./bin/main_gen.o

./bin/tracker.o: ./database_gen/Tracker.cpp ./database_gen/Tracker.hpp
	$(CC) -c ./database_gen/Tracker.cpp $(CFLAGS) -o ./bin/tracker.o
