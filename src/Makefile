OBJS = $(shell find . -name '*.c')

MM:= ./main.c
MT:= ./Teste.c
REP:=  

OBJT=$(subst $(MM), $(REP), $(OBJS))
OBJM=$(subst $(MT), $(REP), $(OBJS))

CC = gcc
COMPILER_FLAGS = -std=c99 -fstack-protector-all
LINKER_FLAGS = -lm
OBJ_NAME = siguel

siguel: $(OBJM)
	rm -f siguel
	$(CC) $(OBJM) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) 

teste: $(OBJT)
	rm -f teste
	$(CC) $(OBJT) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o teste

clean:
	rm -f siguel

