CC = gcc
LIBS = -lgmp

all:

ec: clear
	$(CC) -o ectest ec.c ectest.c $(LIBS)
	$(CC) -o ecspeedtest ec.c ecspeedtest.c $(LIBS)

ectest: ec
	./ectest

ecspeedtest: ec
	./ecspeedtest

clear:
	rm -f ectest
	rm -f ecspeedtest

