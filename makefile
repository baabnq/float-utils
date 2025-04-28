TARGET = main
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default
run: default
	clear
	./$(TARGET)

mandel: Main.o Mandelbrot.c
	$(CC) Mandelbrot.c -c $(CFLAGS)
	$(CC) Mandelbrot.o Float.o
    


main: Main.o Float.o
	$(CC) Main.o Float.o
    
    
Main.o: Main.c
	$(CC) Main.c -c $(CFLAGS)

Float.o: Float.c
	$(CC) Float.c -c $(CFLAGS)

clean:
	-rm -f *.o
	-rm -f $(TARGET)
