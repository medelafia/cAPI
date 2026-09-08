CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LIBS = -lpthread -lsqlite3
TARGET = server
SOURCES = server.c controllers.c db.c config.c common.c cJSON.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run