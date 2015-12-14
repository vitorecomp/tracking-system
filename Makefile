#makefile

#normal flags
#compiler
CC = g++

#flags
CFLAGS =  -w -std=c++11 -fmax-errors=100

#normal libs
LIBS = -pthread -fopenmp

#compile project
all: bin/main.o
	rm -f log.txt
	$(CC) ./bin/*.o $(CFLAGS) -o ./release/tracking_system $(LIBS)

clean:
	rm -r -f ./bin/
	mkdir bin
	rm -r -f ./release/
	mkdir release

bin/main.o: ./source/main.cpp genetic_core io_mananger libs
	$(CC) -c ./source/main.cpp $(CFLAGS) -o ./bin/main.o $(LIBS)

genetic_core: bin/ManangerCore.o bin/Tracklet.o
io_mananger: bin/IOMananger.o bin/IOBasetypes.o bin/IOEntities.o
libs: bin/json.o

#ManangerCore
bin/ManangerCore.o: ./source/src/ManangerCore.cpp ./source/header/ManangerCore.hpp
	$(CC) -c ./source/src/ManangerCore.cpp $(CFLAGS) -o ./bin/ManangerCore.o $(LIBS)

bin/Tracklet.o: ./source/src/Tracklet.cpp ./source/header/Tracklet.hpp
	$(CC) -c ./source/src/Tracklet.cpp $(CFLAGS) -o ./bin/Tracklet.o $(LIBS)

#IOMananger
bin/IOMananger.o: ./source/src/IOMananger.cpp ./source/header/IOMananger.hpp
	$(CC) -c ./source/src/IOMananger.cpp $(CFLAGS) -o ./bin/IOMananger.o $(LIBS)

bin/IOBasetypes.o: ./source/src/Output/BaseTypes.cpp ./source/header/Output/BaseTypes.hpp
	$(CC) -c ./source/src/Output/BaseTypes.cpp $(CFLAGS) -o ./bin/IOBasetypes.o $(LIBS)

bin/IOEntities.o: ./source/src/Output/Entities.cpp ./source/header/Output/Entities.hpp
	$(CC) -c ./source/src/Output/Entities.cpp $(CFLAGS) -o ./bin/IOEntities.o $(LIBS)

#LIBS
#compile json third-party lib
bin/json.o: ./source/libs/jsonParser/json.h ./source/libs/jsonParser/jsoncpp.cpp
	$(CC) -c ./source/libs/jsonParser/jsoncpp.cpp $(CFLAGS) -o ./bin/json.o


##############################################################
#Testes de execução autmatica
##############################################################
#test libs
LIB_TEST = -lcppunit

database_gen: ./database_gen/main.cpp
	$(CC) ./database_gen/main.cpp $(CFLAGS) -o ./release/database_gen $(LIBS)
