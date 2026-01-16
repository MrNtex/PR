CC = gcc
CFLAGS = -Wall -O2

TARGET = calka_omp

SRC = calka_omp.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -lm

clean:
	rm -f $(TARGET)