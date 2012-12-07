
ifndef (CC)
	CC= gcc
endif

CFLAGS= -std=c99

all: tartiflette

tartiflette: main.o main_functions.o processing.o header.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o tartiflette
