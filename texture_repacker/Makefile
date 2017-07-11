CC= gcc
LIBS = 
OBJ = texture.o bmp.o psx.o
CFLAGS = -std=c11 -g -mno-ms-bitfields
all: texture 

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

texture: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf *.o
