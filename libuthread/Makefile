lib := libuthread.a 
objects := queue.o uthread.o 
flags := -Wall -Wextra -Werror

all: $(lib)
$(lib): $(objects)
	ar rcs $(lib) $(objects)

queue.o: queue.c queue.h
	gcc	$(flags) -c  queue.c

uthread.o: uthread.c uthread.h
	gcc	$(flags) -c  uthread.c
	
clean:
	rm -f $(lib)
