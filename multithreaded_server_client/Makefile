CC=gcc
CFLAGS=-Wall -g -lpthread
MAIN=server
OBJECTS=server.o 
all: $(MAIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN): $(OBJECTS)
	$(CC) $^ -lpthread -o $@

.PHONY: clean
clean:
	rm -f $(MAIN) $(OBJECTS)
