CC=i686-w64-mingw32-gcc
STRIP=i686-w64-mingw32-strip
LIBS = 
OBJ = psx.o bmp.o
CFLAGS = -std=c11 -O2 -static -g -mno-ms-bitfields

all: strip 

strip: psx
	$(STRIP) psx.exe

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

psx: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf $(OBJ) psx.exe
