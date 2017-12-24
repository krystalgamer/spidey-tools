CC= gcc
LIBS = 
OBJ = texture_loader.o memory.o patches.o
CFLAGS = -std=c11 -g
all: texture 

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

texture: $(OBJ)
		gcc -shared -o $@.dll $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf *.o
