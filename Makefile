all: main.c function.h process.c scheduling.c
	gcc main.c process.c scheduling.c -o main

clean:
	rm -f main
