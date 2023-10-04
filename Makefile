srcdir = .
CC = gcc
CFLAGS = -g -O2 -W -Wall -I.
LDFLAGS = -static
LIBS = -lutil 
VERSION = 0.9


OBJ = attach.o master.o main.o
SRC = $(srcdir)/attach.c $(srcdir)/master.c $(srcdir)/main.c

dtach: $(OBJ)
	$(CC) -o $@ $(LDFLAGS) $(OBJ) $(LIBS)
	mv dtach plugctl

clean:
	rm -f plugctl $(OBJ)

attach.o: ./attach.c ./dtach.h config.h
master.o: ./master.c ./dtach.h config.h
main.o: ./main.c ./dtach.h config.h

