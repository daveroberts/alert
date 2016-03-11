CC=gcc

compile: alert.c
	$(CC) alert.c -o alert `pkg-config --cflags --libs gtk+-2.0`
