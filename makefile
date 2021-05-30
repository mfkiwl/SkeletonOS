CC=gcc

INCLUDE=include
CORE_OBJECT_DIR=skeleton
CORE_OBJECT=scheduler.o cbuf.o logger.o json.o config.o fs.o uuid4.o timestamp.o math.o

CFLAGS= -I$(INCLUDE) -Wall -Wno-unused-variable -Wno-unused-function -O0
LIBEXT= -lm

SKELETON=main

_OBJ_CORE = $(CORE_OBJECT)
OBJ_CORE = $(patsubst %,$(CORE_OBJECT_DIR)/%,$(_OBJ_CORE))

$(SKELETON): $(OBJ_CORE) $(OBJ_MODEL) $(SKELETON).o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBEXT)


all: SKELETON

.PHONY: clean

CLEAN_LIST += $(OBJ_CORE) $(OBJ_MODEL) $(SKELETON) $(SKELETON).o \
    $(patsubst %.o,%.d,$(OBJS))

clean:
	rm -f $(CLEAN_LIST)
	rm -rf ./log/*.*
	rm -rf ./data/*
