CC= gcc
LIBS = 
OBJ = pkr.o walk.o
CFLAGS = -std=c11 -g
all: pkr 

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

pkr: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf *.o
