lib := libuthread.a 
objects := queue.o uthread.o context.o #sem.o
flags := -Wall -Wextra -Werror -g

all: $(lib)
$(lib): $(objects)
	ar rcs $(lib) $(objects)

queue.o: queue.c queue.h
	gcc	$(flags) -c  queue.c

context.o: context.c private.h
	gcc	$(flags) -c  context.c

uthread.o: uthread.c uthread.h private.h queue.h
	gcc	$(flags) -c  uthread.c
	
# sem.o: sem.c sem.h private.h queue.h
# 	gcc $(flags) -c  sem.c

clean:
	rm -f $(lib)
