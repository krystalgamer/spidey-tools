CC=i686-w64-mingw32-gcc
STRIP=i686-w64-mingw32-strip
LIBS = -lz
OBJ = pkr.o walk.o extract.o
CFLAGS = -std=c11 -O2 -static

all: strip 

strip: pkr
		$(STRIP) pkr

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

pkr: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf $(OBJ) pkr.exe
