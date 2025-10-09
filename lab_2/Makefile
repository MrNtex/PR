# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

OPT = -O0 -g -DDEBUG

all: debug

debug: OPT = -O0 -g -DDEBUG
debug: fork clone

opt: OPT = -O3 -DNDEBUG
opt: fork clone

# pliki naglowkowe
INC = -I../pomiar_czasu

# biblioteki
LIB = -L../pomiar_czasu -lpomiar_czasu -lm

# zaleznosci i komendy
fork: fork.o 
	$(LOADER) $(OPT) fork.o -o fork $(LIB)

# jak uzyskac plik fork.o ?
fork.o: fork.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) fork.c $(INC) 

# zaleznosci i komendy
clone: clone.o 
	$(LOADER) $(OPT) clone.o -o clone $(LIB)

# jak uzyskac plik clone.o ?
clone.o: clone.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) clone.c $(INC) 

clean:
	rm -f *.o fork clone
