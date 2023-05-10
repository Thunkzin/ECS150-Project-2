lib := libuthread.a 
objects := queue.o uthread.o context.o# sem.o preempt.o 
flags := -Wall -Wextra -Werror -g

all: $(lib)
$(lib): $(objects)
	ar rcs $(lib) $(objects)

queue.o: queue.c queue.h
	gcc	$(flags) -c  queue.c

uthread.o: uthread.c uthread.h private.h queue.h
	gcc	$(flags) -c  uthread.c

context.o: context.c private.h uthread.h
	gcc	$(flags) -c  context.c

# sem.o: sem.c sem.h private.h queue.h uthread.h
# 	gcc $(flags) -c  sem.c

# preempt.o: preempt.c private.h uthread.h
# 	gcc $(flags) -c preempt.c

clean:
	rm -f $(lib)
