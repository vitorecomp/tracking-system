#makefile

#normal flags
#compiler
CC = g++

#flags
CFLAGS =  -w -std=c++0x -fmax-errors=100

#normal libs
LIBS = -pthread

#compile project
all: bin/main.o
	rm -f log.txt
	$(CC) ./bin/*.o $(CFLAGS) -o ./release/geneticTraining $(LIBS)

clean:
	rm -r -f ./bin/
	mkdir bin
	rm -r -f ./release/
	mkdir release

bin/main.o: ./source/main.cpp mananger_core io_mananger libs
	$(CC) -c ./source/main.cpp $(CFLAGS) -o ./bin/main.o $(LIBS)

mananger_core: bin/ManangerCore.o
io_mananger: bin/IOMananger.o
libs: bin/json.o

#ManangerCore
bin/ManangerCore.o: ./source/src/ManangerCore.cpp ./source/header/ManangerCore.hpp
	$(CC) -c ./source/src/ManangerCore.cpp $(CFLAGS) -o ./bin/ManangerCore.o $(LIBS)

bin/Specimen.o: ./source/src/Specimen.cpp ./source/header/Specimen.hpp
	$(CC) -c ./source/src/Specimen.cpp $(CFLAGS) -o ./bin/Specimen.o $(LIBS)

#IOMananger
bin/IOMananger.o: ./source/src/IOMananger.cpp ./source/header/IOMananger.hpp
	$(CC) -c ./source/src/IOMananger.cpp $(CFLAGS) -o ./bin/IOMananger.o $(LIBS)

#LIBS
#compile json third-party lib
bin/json.o: ./source/libs/jsonParser/json.h ./source/libs/jsonParser/jsoncpp.cpp
	$(CC) -c ./source/libs/jsonParser/jsoncpp.cpp $(CFLAGS) -o ./bin/json.o


##############################################################
#Testes de execução autmatica
##############################################################
#test libs
LIB_TEST = -lcppunit

#compile logger test
testLogger: clean ./testes/loggerTest/loggerTest.cpp ./testes/loggerTest/main.cpp bin/logger.o
	-ipcrm -Q 0x2587
	-rm log.txt
	$(CC) -c ./testes/loggerTest/loggerTest.cpp $(CFLAGS) -o ./bin/loggerTest.o $(LIBS) $(LIB_TEST)
	$(CC) -c ./testes/loggerTest/main.cpp $(CFLAGS) -o ./bin/loggerTestMain.o $(LIBS) $(LIB_TEST)
	$(CC) ./bin/*.o $(CFLAGS) -o ./release/loggerTest $(LIBS) $(LIB_TEST)

#compile json entities test
testJson: clean ./testes/jsonTest/jsonTest.cpp ./testes/jsonTest/main.cpp bin/logger.o bin/entities.o
	$(CC) -c ./testes/jsonTest/jsonTest.cpp $(CFLAGS) -o ./bin/jsonTest.o $(LIBS) $(LIB_TEST)
	$(CC) -c ./testes/jsonTest/main.cpp $(CFLAGS) -o ./bin/jsonTestMain.o $(LIBS) $(LIB_TEST)
	$(CC) ./bin/*.o $(CFLAGS) -o ./release/jsonTest $(LIBS) $(LIB_TEST)

#compile entities/basetypes tests
testEntities: clean ./testes/entitiesTest/entitiesTest.cpp ./testes/entitiesTest/entitiesTest.hpp ./testes/entitiesTest/main.cpp bin/entities.o
	$(CC) -c ./testes/entitiesTest/entitiesTest.cpp $(CFLAGS) -o ./bin/entitiesTest.o $(LIBS) $(LIB_TEST)
	$(CC) -c ./testes/entitiesTest/main.cpp $(CFLAGS) -o ./bin/entitiesTestMain.o $(LIBS) $(LIB_TEST)
	$(CC) ./bin/*.o $(CFLAGS) -o ./release/entitiesTest $(LIBS) $(LIB_TEST)

#compile database test
testDatabase: clean ./testes/databaseTestes/databaseTest.cpp ./testes/databaseTestes/main.cpp bin/database.o
	$(CC) -c ./testes/databaseTestes/databaseTest.cpp $(CFLAGS) -o ./bin/databaseTest.o $(LIBS) $(LIB_TEST)
	$(CC) -c ./testes/databaseTestes/main.cpp $(CFLAGS) -o ./bin/databaseTestMain.o $(LIBS) $(LIB_TEST)
	$(CC) ./bin/*.o $(CFLAGS) -o ./release/databaseTest $(LIBS) $(LIB_TEST)
