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

sha256:
	$(CC) -o sha256test sha256.c sha256test.c $(LIBS)

sha256test: sha256
	./sha256test

clear:
	rm -f ectest
	rm -f ecspeedtest

