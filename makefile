CC=gcc

INCLUDE=include
CORE_OBJECT_DIR=skeleton
CORE_OBJECT=scheduler.o cbuf.o logger.o json.o config.o fs.o uuid4.o timestamp.o
MATH_OBJECT=math.o

CFLAGS= -I$(INCLUDE) -Wall -Wno-unused-variable -Wno-unused-function -O0
LIBEXT= -lm

SKELETON=main

_OBJ_CORE = $(CORE_OBJECT)
OBJ_CORE = $(patsubst %,$(CORE_OBJECT_DIR)/%,$(_OBJ_CORE))

_OBJ_MATH = $(MATH_OBJECT)
OBJ_MATH = $(patsubst %,$(CORE_OBJECT_DIR)/%,$(_OBJ_MATH))

$(SKELETON): $(OBJ_CORE) $(OBJ_MATH) $(SKELETON).o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBEXT)


all: SKELETON

LIB_OUTPUT_DIR=lib
LIB_NAME=libSkeleton

LIB=lib

$(LIB): $(OBJ_CORE)
	mkdir -p $(LIB_OUTPUT_DIR)
	$(AR) rvs $(LIB_NAME).a $^
	mv $(LIB_NAME).a $(LIB_OUTPUT_DIR)/
	cp $(INCLUDE)/skeleton/*.h $(LIB_OUTPUT_DIR)/
	rm -f $(LIB_OUTPUT_DIR)/math.h


.PHONY: clean cleanLib cleanAll

CLEAN_LIST += $(OBJ_CORE) $(OBJ_MATH) $(SKELETON) $(SKELETON).o \
    $(patsubst %.o,%.d,$(OBJS))

clean:
	rm -f $(CLEAN_LIST)
	rm -rf ./log/*.*
	rm -rf ./data/*


CLEAN_LIB_LIST += $(OBJ_CORE) $(LIB_NAME).a $(LIB_OUTPUT_DIR)/*

cleanLib:
	rm -f $(CLEAN_LIB_LIST)

cleanAll:
	rm -f $(CLEAN_LIST) $(CLEAN_LIB_LIST)