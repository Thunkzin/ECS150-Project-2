lib := libuthread.a 
objects := queue.o
flags := -Wall -Wextra -Werror

all: $(lib)
$(lib): $(objects)
	ar rcs $(lib) $(objects)

queue.o: queue.c queue.h
	gcc  $(flags) -c  queue.c
	
clean:
	rm -f $(lib)
