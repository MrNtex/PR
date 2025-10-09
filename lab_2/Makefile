# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

OPT = -O0 -g -DDEBUG

all: debug

debug: OPT = -O0 -g -DDEBUG
debug: fork clone clone_2 program
debug: clone_3

opt: OPT = -O3 -DNDEBUG
opt: fork clone clone_2 program
opt: clone_3

# pliki naglowkowe
INC = -I../pomiar_czasu

# biblioteki
LIB = -L../pomiar_czasu -lpomiar_czasu -lm

# zaleznosci i komendy
fork: fork.o 
	$(LOADER) $(OPT) fork.o -o fork ../pomiar_czasu/libpomiar_czasu.a -lm

# jak uzyskac plik fork.o ?
fork.o: fork.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) fork.c $(INC) 

# zaleznosci i komendy
clone: clone.o 
	$(LOADER) $(OPT) clone.o -o clone ../pomiar_czasu/libpomiar_czasu.a -lm

# jak uzyskac plik clone.o ?
clone.o: clone.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone.c $(INC) 

clone_2: clone_2.o 
	$(LOADER) $(OPT) clone_2.o -o clone_2 ../pomiar_czasu/libpomiar_czasu.a -lm

clone_2.o: clone_2.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone_2.c $(INC) 

program: program.o
	$(LOADER) $(OPT) program.o -o program

program.o: program.c
	$(CCOMP) -c $(OPT) program.c

clone_3: clone_3.o
	$(LOADER) $(OPT) clone_3.o -o clone_3 ../pomiar_czasu/libpomiar_czasu.a -lm

clone_3.o: clone_3.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone_3.c $(INC) 

clean:
	rm -f *.o fork clone clone_2 program clone_3
