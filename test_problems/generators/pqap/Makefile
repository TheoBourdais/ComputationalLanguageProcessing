CC=gcc
CFLAGS=-Wall -I./sources -g
LDFLAGS=-lm
SOURCES=sources/gener6.c sources/main_GEN.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=gen

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)