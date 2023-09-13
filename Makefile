CC = gcc
CFLAGS = -pthread

SOURCE_FILES = $(wildcard source/*.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCE_FILES))

webserver: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) webserver