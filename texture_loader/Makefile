CC= gcc
AC= nasm
LIBS = 
OBJ = loader.o memory.o patches.o md5.o
AOBJ = hooks.obj
CFLAGS = -static -std=gnu11 -g
all: texture 

%.o: %.c
		$(CC) -c -o $@ $< $(LIBS) $(CFLAGS)

texture: $(OBJ) $(AOBJ)
		gcc -shared -o $@.dll $^ $(CFLAGS) $(LIBS)

%.obj: %.s
		nasm $< -f win -o $@
clean:
		rm -rf *.o *.obj

install:
	mv texture.dll /e/Activision/Spider-Man/binkw32.dll
