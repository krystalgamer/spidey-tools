CC= gcc
LIBS = 
OBJ = gui.o
CFLAGS = -mwindows -std=c11 -g 
all: gui 

%.o: %.c
		$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

gui: $(OBJ)
		gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
		rm -rf *.o
