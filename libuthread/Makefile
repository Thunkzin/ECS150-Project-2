# Target library
lib := libuthread.a
objects := queue.o
flags := -Wall -Wextra -Werror

all: $(lib)


queue.o: queue.c queue.h
	gcc  $(flags) -c  queue.c
	
	
$(lib): $(objects)
	ar rcs $(lib) $(objects)

clean:
	rm -f $(lib)
