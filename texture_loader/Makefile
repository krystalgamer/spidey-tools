CC=i686-w64-mingw32-gcc-win32
STRIP=i686-w64-mingw32-strip
AC= nasm
LIBS = 
OBJ = loader.o memory.o patches.o md5.o cJSON.o
AOBJ = hooks.obj
CMINUS = -std=c11 -static
CFLAGS = $(CMINUS) -Os

all: strip 

strip : texture
	$(STRIP) texture.dll


loader.o: loader.c
		$(CC) -c -o $@ $< $(LIBS) $(CMINUS)

%.o: %.c
		$(CC) -c -o $@ $< $(LIBS) $(CFLAGS)

texture: $(OBJ) $(AOBJ)
		$(CC) -shared -o $@.dll $^ $(CFLAGS) $(LIBS)

%.obj: %.s
		nasm $< -f win -o $@
clean:
		rm -rf $(OBJ) $(AOBJ) texture.dll
