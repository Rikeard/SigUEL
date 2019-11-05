OBJS = $(shell find . -name '*.c')

MM:= ./main.c
MV:= 
REP:=  

OBJM=$(subst $(MV), $(REP), $(OBJS))
OBJV=$(subst $(MM), $(REP), $(OBJS))

CC = gcc
COMPILER_FLAGS = -std=c99 -fstack-protector-all
LINKER_FLAGS = -lm
OBJ_NAME = siguel

siguel: $(OBJM)
	rm -f siguel
	$(CC) $(OBJM) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) 

visibility: $(OBJV)
	rm -f visibility
	$(CC) $(OBJV) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o visibility

all: $(OBJS)
	rm -f siguel
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o siguel

clean:
	rm -f siguel

