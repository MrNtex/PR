CC = mpicc
CFLAGS = -Wall -O2

TARGET = MPI_PI

SRC = MPI_PI.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -lm

clean:
	rm -f $(TARGET)