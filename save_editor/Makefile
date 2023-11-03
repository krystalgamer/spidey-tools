CC=i686-w64-mingw32-gcc
STRIP=i686-w64-mingw32-strip
LIBS = 
OBJ = save.o 
CFLAGS = -std=c11 -g -O2 -static

all: strip 

strip: save
		$(STRIP) save.exe

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

save: $(OBJ)
		$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf $(OBJ) save.exe
