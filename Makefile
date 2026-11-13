CFLAGS = -Wall -Wextra -std=c99 $(shell pkg-config --cflags raylib)
LIBS = $(shell pkg-config --libs raylib)

TARGET = main
SRC = main.c

all: $(TARGET) 

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LIBS) -o $(TARGET)

Clean:
	rm -f $(TARGET)


