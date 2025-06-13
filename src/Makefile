CC = gcc
CFLAGS = -Wall -O2 -std=c99
TARGET = bf
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET) test.bf

clean:
	rm -f $(TARGET)
