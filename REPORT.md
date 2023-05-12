# ECS150_Project#2: User-level thread libray

## Summary
This program is a basic `user-level thread` library that provides an interface for 
applications to create and run independent threads concurrenlty. This library also 
provides features such as semaphores, preemption, timer, and signal handlers to 
make sure the two threads do not encounter any `race conditions` and synchronize with 
each other. 

The `queue` API is implmented to manage the execution order of the threads. The `uthread` 
API is used to create and mangae threads at the user levles. The `semaphores` API is used 
to synchronize access to shared resources between threads. `Preemption` is a feature that 
allows the OS to interrupt a running thread and switch to another thread. 


### Phase 1: queue API



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
 




### Debugging issues






### Resources

https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Signal-Actions
https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Setting-an-Ala
rm
