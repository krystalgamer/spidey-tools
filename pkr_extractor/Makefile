CC= gcc
LIBS = -lz
OBJ = pkr.o walk.o extract.o
CFLAGS = -std=c11 -g -static
all: pkr 

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS)

pkr: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf *.o
