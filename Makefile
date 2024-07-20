CC=gcc
CFLAGS=-ggdb -Wall -Wextra -O0
OBJ = my_shell1.6.o -lm

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<
	
myshell: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *~ myshell_Hessman
