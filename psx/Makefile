CC= gcc
LIBS = 
OBJ = psx.o 
CFLAGS = -std=c11 -g
all: psx 

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

psx: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf *.o
