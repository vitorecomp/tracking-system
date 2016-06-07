CC = g++
HEADER_PATH = source/header
SRC_PATH = source
BIN_PATH = bin
REL_PATH = release
DEP_PATH = dep

_OBJ = main.o

_OBJ += ./libs/jsonParser/jsoncpp.o
_OBJ += ./libs/output_s/IOMananger.o
_OBJ += ./libs/output_s/IOMananger_Local.o
_OBJ += ./libs/output_s/BaseTypes.o
_OBJ += ./libs/output_s/Entities.o


OBJ = $(patsubst %,$(BIN_PATH)/%,$(_OBJ))

DIRECTIVES = -std=c++0x -w -c -I $(HEADER_PATH)  `pkg-config --cflags opencv` -O4
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.Td
OPENCV = `pkg-config --libs opencv` -O4
LIBS = $(OPENCV)


FINAL_EXEC = $(REL_PATH)/database_gen
all: $(FINAL_EXEC)
$(FINAL_EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LIBS)

$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp
	@mkdir -p $(BIN_PATH)/$(dir $*)
	@mkdir -p $(DEP_PATH)/$(dir $*)
	@mkdir -p $(BIN_PATH) $(DEP_PATH)
	$(CC) $(DEPFLAGS) -c -o $@ $< $(DIRECTIVES)
	@mv -f $(DEP_PATH)/$*.Td $(DEP_PATH)/$*.d

debug: DIRECTIVES += -ggdb
debug: all

.PHONY: clean
clean:
	-@rm -r $(BIN_PATH)

$(DEP_PATH)/%.d: ;
.PRECIOUS: $(DEP_PATH)/%.d

-include $(patsubst %,$(DEP_PATH)/%.d,$(basename $(_OBJ)))


#################
##LIBS
#################

#liberies: bin/json.o output
bin/json.o: ./source/libs/jsonParser/json.h ./source/libs/jsonParser/jsoncpp.cpp
	$(CC) -c ./source/libs/jsonParser/jsoncpp.cpp $(DIRECTIVES) -o ./bin/json.o

############################################################################################################
#output - compilation
############################################################################################################
output: bin/IOMananger.o bin/IOManangerL.o bin/IOBasetypes.o bin/IOEntities.o

bin/IOMananger.o: ./source/libs/output_s/IOMananger.cpp ./source/libs/output/IOMananger.hpp
	$(CC) -c ./source/libs/output_s/IOMananger.cpp $(DIRECTIVES) -o ./bin/IOMananger.o

bin/IOManangerL.o: ./source/libs/output_s/IOMananger_Local.cpp ./source/libs/output/IOMananger.hpp
	$(CC) -c ./source/libs/output_s/IOMananger_Local.cpp $(DIRECTIVES) -o ./bin/IOManangerL.o

bin/IOBasetypes.o: ./source/libs/output_s/BaseTypes.cpp ./source/libs/output//BaseTypes.hpp
	$(CC) -c ./source/libs/output_s/BaseTypes.cpp $(DIRECTIVES) -o ./bin/IOBasetypes.o

bin/IOEntities.o: ./source/libs/output_s/Entities.cpp ./source/libs/output/Entities.hpp
	$(CC) -c ./source/libs/output_s/Entities.cpp $(DIRECTIVES) -o ./bin/IOEntities.o
