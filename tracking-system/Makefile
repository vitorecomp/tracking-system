CC = g++
HEADER_PATH = source/header
SRC_PATH = source
BIN_PATH = bin
REL_PATH = release
DEP_PATH = dep
LOG_PATH = log
RESULT_PATH = results

RELEASE_NAME = tracking-system

_OBJ = main.o

_OBJ += ./libs/jsonParser/jsoncpp.o
_OBJ += ./libs/output_s/IOMananger.o
_OBJ += ./libs/output_s/IOMananger_Local.o
_OBJ += ./libs/output_s/BaseTypes.o
_OBJ += ./libs/output_s/Entities.o

_OBJ += ./src/ManangerCore.cpp
_OBJ += ./src/queue/queue.cpp


_OBJ += ./libs/output_s/Entities.o

OBJ = $(patsubst %,$(BIN_PATH)/%,$(_OBJ))

DIRECTIVES = -std=c++0x -w -c -I $(HEADER_PATH)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.Td
OPENCV = `pkg-config --libs opencv` -O4
LIBS = $(OPENCV)


FINAL_EXEC = $(REL_PATH)/$(RELEASE_NAME)
all: $(FINAL_EXEC)
$(FINAL_EXEC): $(OBJ)
	@mkdir -p $(RESULT_PATH)
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
	-@rm -r $(DEP_PATH)
	-@rm -r $(REL_PATH)
	-@rm -r $(LOG_PATH)

$(DEP_PATH)/%.d: ;
.PRECIOUS: $(DEP_PATH)/%.d

-include $(patsubst %,$(DEP_PATH)/%.d,$(basename $(_OBJ)))

