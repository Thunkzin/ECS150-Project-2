lib := libuthread.a 
objects := queue.o #uthread.o context.o
flags := -Wall -Wextra -Werror

all: $(lib)
$(lib): $(objects)
	ar rcs $(lib) $(objects)

queue.o: queue.c queue.h
	gcc	$(flags) -c  queue.c

# context.o: context.c private.h
#	gcc	$(flags) -c  context.c

# uthread.o: uthread.c uthread.h private.h queue.h
#	gcc	$(flags) -c  uthread.c
	
clean:
	rm -f $(lib)
