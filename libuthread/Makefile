# Target library
lib := libuthread.a

all: $(lib)
	
lib: queue.o
	ar rcs lib queue.o

queue.o: queue.c
	gcc -Wall -Wextra -Werror -c -o queue.o queue.c
	

clean:
	rm -f lib 
