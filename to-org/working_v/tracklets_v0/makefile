#makefile

#normal flags
CC = g++
CFLAGS = -W -std=c++0x `pkg-config --cflags opencv`
LIBS = -pthread `pkg-config --libs opencv` -O4

#compile project

all: ./bin/main.o ./bin/Brain.o ./bin/thread.o ./bin/Tracking.o ./bin/opencvtemplateMacth.o
	$(CC) ./bin/*.o ./libs/*.cpp $(CFLAGS) -o ./release/track $(LIBS)

./bin/main.o: ./sources/main.cpp ./bin/Tracking.o ./bin/Entities.o ./bin/Brain.o ./bin/thread.o
	$(CC) -c ./sources/main.cpp $(CFLAGS) -o ./bin/main.o

#brain
./bin/Brain.o: ./sources/Brain.cpp ./bin/Entities.o
	$(CC) -c ./sources/Brain.cpp $(CFLAGS) -o ./bin/Brain.o

./bin/thread.o: ./sources/thread.cpp
	$(CC) -c ./sources/thread.cpp $(CFLAGS) -o ./bin/thread.o

#tracklet
./bin/Tracking.o: ./sources/Tracking.cpp ./bin/Entities.o ./bin/opencvtemplateMacth.o ./bin/fullTM.o
	$(CC) -c ./sources/Tracking.cpp $(CFLAGS) -o ./bin/Tracking.o

./bin/opencvtemplateMacth.o: ./sources/templateMatch/opencvTemplate.cpp ./bin/Entities.o
	$(CC) -c ./sources/templateMatch/opencvTemplate.cpp $(CFLAGS) -o ./bin/opencvtemplateMacth.o

./bin/fullTM.o: ./sources/templateMatch/fullTM.cpp ./bin/Entities.o
	$(CC) -c ./sources/templateMatch/fullTM.cpp $(CFLAGS) -o ./bin/fullTM.o


#aquivos basicos
./bin/Entities.o: ./sources/Entities.cpp ./bin/BaseTypes.o
	$(CC) -c ./sources/Entities.cpp $(CFLAGS) -o ./bin/Entities.o

./bin/BaseTypes.o: ./sources/BaseTypes.cpp
	$(CC) -c ./sources/BaseTypes.cpp $(CFLAGS) -o ./bin/BaseTypes.o

clean:
	-rm bin/*
