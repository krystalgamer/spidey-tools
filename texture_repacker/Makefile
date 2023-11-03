CC=i686-w64-mingw32-gcc
STRIP=i686-w64-mingw32-strip
LIBS = 
OBJ = texture.o bmp.o psx.o
CFLAGS = -std=c11 -g -mno-ms-bitfields

all: strip 

strip: texture
		$(STRIP) texture.exe

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

texture: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf $(OBJ) texture.exe
