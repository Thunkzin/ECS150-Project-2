# ECS150_Project#2: User-level thread libray

## Summary
This program is a basic `user-level thread` library that provides an interface for
applications to create and run independent threads concurrently. This library also
provides features such as semaphores, preemption, timer, and signal handlers to
make sure the two threads do not encounter any `race conditions` and synchronize with
each other.

The `queue` API is implemented to manage the execution order of the threads. The 
`uthread` API is used to create and manage threads at the user levels. The 
`semaphores` API is used to synchronize access to shared resources between threads. 
`Preemption` is a feature that allows the OS to interrupt a running thread and switch 
to another thread.

## Phase 1: queue API
The queue API we implemented uses a linked list data structure to achieve a FIFO 
method where data is enqueued one after another. First, we declare a struct to store 
the nodes' data `element_data_address` and the address of the next node`next_element` 
using the pointers. The first element in the linked list is called `first` and the 
last element is called `last.` 

The function `queue_create()` allocates an empty queue dynamically using `malloc`. 
Since the data size is unknown, we use malloc to prevent the error of running out of 
memory when creating a new queue. The queue is initially empty and there is no data 
stored inside. So, we set its `queue_size` to be zero and the first and last pointers 
of the queue to point to NULL. The function `queue_destroy()` deallocates the memory 
created for the queue from the heap. If the queue is non-empty, it will free the 
memory consumed by the queue using `free(queue)` and otherwise, it will return -1. 

When a thread calls `queue_enqueue()` function, it is added to the very end of the 
queue. When a thread calls `queue_dequeue()`function, the queue removes the first 
(oldest) thread in the queue and stores its data in the memory pointed by @data. 

The function `queue_delete()`is responsible for deleting the oldest thread in the 
queue that contains the pointer data which is passed as the function’s arguments. We 
use a while loop to iterate through every item in the queue to find the data. If the 
data is found, it will delete the data by using `free(current)` and -1 if it fails.
 
The function `queue_iterate()` provides a way to call a function within its own. It 
has two parameters: `queue` to iterate over and `func` to be called on each item of 
the queue. The function iterates starting from the oldest up until the newly added 
item using if statement.

### Phase 2: uthread API
The uthread API library in this program creates a new threads, and store each 
thread in the queue by including the queue_t API we creates in previous phase.

Firstly, we define two queue, to store the threads that are still alive (can be
changed between states running, ready, and blocked), and the threads that will 
never go back to above states(store the thread with zombie state).

The very first function that will be called in the main to implement the 
uthread is uthread_run(the thread function), which create an idle thread, that 
keep yielding, and the first thread that will act as current thread.

Then, it will enter the while loop that keeps yielding the very first threads 
in the alive queue by changing the current thread's state, takes a snap shot of 
current thread's context, then apply enqueue and dequeue function to store the
current threads and pop out next threads. 

The second function that will be applied mainly is uthread_create(the thread 
function to be create), which will literally create a thread, allocate the 
memory for itself and its context, then set its state to ready, finally enqueue 
it to the alive queue.

The last function is uthread_exit(), which terminate the current running thread,
setting its state into zombie, and enquqe it into zombie queue.


### Phase 3: semaphore API




### Phase 4: preemption



### test_preempt.c

The test_preempt.c that we made was modded from the uthread_yield.c. Firstly we   
set the boolean parameter in the uthread_run to true, and create a thread1 that 
will enter an infinite loop that prints "1" repeatly.  

In the thread1, it will create the other thread2 which is almost identical to 
thread1, but create the new thread calls thread3, and prints"2" instead. 
Unlike thread1 and thread2, thread3 will only prints a message that shows 
program entered thread3, then terminateds the program.

Since we didn't call any uthread_yield function in our test program, we can 
guarantee that if the program successfully calls yield function, it's from the 
signal handler in preempt.c when it received the SIGVTALRM signal from timer.

Therefore, if the program is working without issues, then test_preempt should
perform as below:

1.  Action: uthread_run(thread1), Currently running thread1 and print tons of 1, 
Alive_queue: empty

2.  Action: uthread_create(thread2), Currently running thread1 and print tons 
of 1, Alive_queue: thread2

3.  Action: uthread_yields() by signal handlel, Currently running thread2 and 
print tons of 2, Alive_queue: thread1

4.  Action: uthread_create(thread3), Currently running thread2 and print tons of
2, Alive_queue: thread1 - thread3

5.  Action: uthread_yields() by signal handler, Currently running thread1 and 
print tons of 1, Alive_queue: thread3 - thread2

6.  Action: uthread_yields() by signal handler, Currently running thread3, print
 the message and exit the test, Alive_queue: thread2 - thread1

### Debugging issues

We didn't encountered any serious bugs on queue section, we can simply make and 
run the test with it. However, when we includes the uthread.c the compiler 
comes out with following error messages:

error: The deprecated ucontext routines require _XOPEN_SOURCE to be defined

Nevertheless, our code didn't get the same issues running gradescope's test, so
we consider it being the issues due to the local OS in our system.

Thus, we debug our code mostly with gradescope's error message and CSIF's 
system.

We encountered the first bug on phase 2, which was the segmantation fault while
testing uthread_hello.c.

The bug was caused by the inapproprate use of malloc() function, in 
uthread_create(), which didn't takes us a lot of time to deal with.

The second bugs that we encountered was the one that bother us the most, which 
our uthread_hello.c and uthread_yield.c test doesn't print out any message.

It doesn't take us much time that it's because we didn't apply queue_create 
function to our global queue: alive_queue, and zombie_queue. However, when we 
call the queue_create function, the test entered a infinite loop which lead to 
a timeout condition.

Finally, we found it was inappropriately setting the idle_thread's state into 
ready that caused the bug, since the idle_thread will stuck in alive_queue, 
which makes the program stucked in the while loops, and the code works 
perfectly fine after we delete that extra line.

### Resources

https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Signal-Actions
https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Setting-an-Ala
rm
