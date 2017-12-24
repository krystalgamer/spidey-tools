CC= gcc
LIBS = 
OBJ = trg.o
CFLAGS = -std=c11 -g 
all: trg 

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

pkr: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf *.o
