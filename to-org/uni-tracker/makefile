#makefile

#normal flags
CC = g++
CFLAGS = -std=c++11 -Wall `pkg-config --cflags opencv` -O4
LIBS = -pthread `pkg-config --libs opencv` -lopencv_features2d

##################################################################################
##compile project
##################################################################################
all: ./bin/main.o
	$(CC) ./bin/*.o ./libs/*.cpp $(CFLAGS) -o ./release/track $(LIBS)

./bin/main.o: ./sources/main.cpp mainDependences
	$(CC) -c ./sources/main.cpp $(CFLAGS) -o ./bin/main.o

mainDependences: basics brain tracks
basics: ./bin/Entities.o ./bin/BaseTypes.o ./bin/Thread.o
brain: ./bin/Brain.o
tracks: ./bin/Tracker.o ./bin/TemplateMatchTrack.o ./bin/SiftTracker.o ./bin/SurfTracker.o

##################################################################################
##Basics
##################################################################################

./bin/Entities.o: ./sources/Entities.cpp ./bin/BaseTypes.o
	$(CC) -c ./sources/Entities.cpp $(CFLAGS) -o ./bin/Entities.o

./bin/BaseTypes.o: ./sources/BaseTypes.cpp
	$(CC) -c ./sources/BaseTypes.cpp $(CFLAGS) -o ./bin/BaseTypes.o

./bin/Thread.o: ./sources/thread.cpp
	$(CC) -c ./sources/thread.cpp $(CFLAGS) -o ./bin/Thread.o

##################################################################################
##Brain
##################################################################################
./bin/Brain.o: ./sources/Brain.cpp
	$(CC) -c ./sources/Brain.cpp $(CFLAGS) -o ./bin/Brain.o

##################################################################################
##Trackers
##################################################################################
./bin/Tracker.o: ./headers/Thread.hpp ./sources/Tracker.cpp
	$(CC) -c ./sources/Tracker.cpp $(CFLAGS) -o ./bin/Tracker.o

./bin/TemplateMatchTrack.o: ./sources/TemplateMatchTrack.cpp
	$(CC) -c ./sources/TemplateMatchTrack.cpp $(CFLAGS) -o ./bin/TemplateMatchTrack.o

./bin/SurfTracker.o: ./sources/SurfTracker.cpp
	$(CC) -c ./sources/SurfTracker.cpp $(CFLAGS) -o ./bin/SurfTracker.o

./bin/SiftTracker.o: ./sources/SiftTracker.cpp
	$(CC) -c ./sources/SiftTracker.cpp $(CFLAGS) -o ./bin/SiftTracker.o

##################################################################################
##compile project
##################################################################################


clean:
	-rm bin/*
