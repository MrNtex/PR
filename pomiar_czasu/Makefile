CC = gcc

CFLAGS = -Wall
LDFLAGS = 

SRC = pomiar_czasu.c moj_program.c
OBJ = pomiar_czasu.o moj_program.o

all: opt

debug: CFLAGS += -O0 -g -DPOMIAR_CZASU
debug: moj_program

opt: CFLAGS += -O2 -DPOMIAR_CZASU
opt: moj_program

moj_program: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o moj_program $(LDFLAGS)

pomiar_czasu.o: pomiar_czasu.c pomiar_czasu.h
	$(CC) $(CFLAGS) -c pomiar_czasu.c

moj_program.o: moj_program.c pomiar_czasu.h
	$(CC) $(CFLAGS) -c moj_program.c

clean:
	rm -f *.o moj_program
