


run: build
	./main

build:
	gcc -g main.c -o main -lm

clean:
	-rm main


