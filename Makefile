CC = gcc
LIBS = -lgmp

all:

ec:
	$(CC) -o ectest ec.c ectest.c $(LIBS)

ectest: ec
	./ectest

clear:
	rm -f ectest