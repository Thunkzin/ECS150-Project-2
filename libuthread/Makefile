lib := libuthread.a 
objects := queue.o uthread.o context.o preempt.o sem.o
CFLAGS := -Wall -Wextra -Werror -g

all: $(lib)
$(lib):  $(objects)
	ar rcs $(lib) $(objects)

queue.o: queue.c queue.h
	gcc	$(CFLAGS) -c  queue.c

uthread.o: uthread.c uthread.h private.h queue.h 
	gcc	$(CFLAGS) -c  uthread.c

context.o: context.c private.h uthread.h 
	gcc	$(CFLAGS) -c  context.c

sem.o: sem.c sem.h private.h queue.h uthread.h
	gcc	$(CFLAGS) -c  sem.c

preempt.o: preempt.c private.h uthread.h
	gcc	$(CFLAGS) -c  preempt.c

clean:
	rm -f $(lib) $(objects)
