# CC=arm-linux-gnueabi-gcc
CC=gcc
CCFlags=
INCLUDES=
LFLAGS=-L/user/lib/x86_64-linux-gnu
LIBS=-lcurl -lpthread

SRC=requestor.c
OBJ=$(SRC:.c=.o)
MAIN=test

RM=rm -rf

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MAIN): $(OBJ)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN)  $(OBJ) $(LFLAGS) $(LIBS)

all: $(MAIN)

clean:
	$(RM) $(MAIN) *.o *~
