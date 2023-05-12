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
####


### Phase 3: semaphore API




### Phase 4: preemption



### test_preempt.c
 




### Debugging issues






### Resources

https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Signal-Actions
https://www.gnu.org/software/libc/manual/2.36/html_mono/libc.html#Setting-an-Ala
rm
