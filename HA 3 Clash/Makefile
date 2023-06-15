CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Werror -D_XOPEN_SOURCE=700 -c

test: test.o halde.o
    $(CC) -o test test.o halde.o
    
test-ref: halde-ref.o test.o
    $(CC) -o test-ref test.o halde-ref.o
    
halde-debug: halde-debug.o
    $(CC) -o halde-debug halde.o
    
halde-debug.o: halde.c
    $(CC) $(CFLAGS) -g halde.c

test.o: test.c
    $(CC) $(CFLAGS) test.c

halde.o: halde.c
    $(CC) $(CFLAGS) halde.c
    
clean:
    rm -f halde.o test.o test test-ref 
