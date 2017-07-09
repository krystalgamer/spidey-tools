CC= gcc
LIBS = 
OBJ = psx.o bmp.o
CFLAGS = -std=c11 -g -mno-ms-bitfields
all: psx 

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

psx: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf *.o
