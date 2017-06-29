CC= gcc
LIBS = 
OBJ = save.o 
CFLAGS = -std=c11 -g
all: save 

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

save: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf *.o
