#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"


/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

bool enable_preempt = false;

/* this func can block signals of type SIGVTALRM */
void preempt_disable(void)
{
	/* Create a signal set to hold the mask of blocked signals */
    sigset_t block_alarm;

    /* Initialize the signal mask to be empty */
    sigemptyset(&block_alarm);

    /* Add the SIGVTALRM signal to the signal set */
    sigaddset(&block_alarm, SIGVTALRM);

    /* Block the SIGVTALRM signal in the current thread */
    sigprocmask(SIG_BLOCK, &block_alarm, NULL);

    // Disable preemption only if it is currently enabled
    if (enable_preempt) {
        enable_preempt = false;
    }
}

/* this func can unblock signals of type SIGVTALRM */
void preempt_enable(void)
{
	// Create a signal set to hold the mask of unblocked signals
    sigset_t unblock_alarm;

    // Initialize the signal set to be empty
    sigemptyset(&unblock_alarm);

    // Add the SIGVTALRM signal to the signal set
    sigaddset(&unblock_alarm, SIGVTALRM);

    // Unblock the SIGVTALRM signal in the current thread
    sigprocmask(SIG_UNBLOCK, &unblock_alarm, NULL);

    // Enable preemption only if it is currently disabled
    if (!enable_preempt) {
        enable_preempt = true;
    }
}


void preempt_start(bool preempt)
{
/**
 * Private preemption API
 */

/*
 * preempt_start - Start thread preemption
 * @preempt: Enable preemption if true
 *
 * Configure a timer that must fire a virtual alarm at a frequency of 100 Hz and
 * setup a timer handler that forcefully yields the currently running thread.
 *
 * If @preempt is false, don't start preemption; all the other functions from
 * the preemption API should then be ineffective.
 */

	/* if preempt is enabled */
	if (preempt) {
		// Create a signal action to handle SIGVTALRM signals 
		struct sigaction action;
		
		action.sa_handler = &uthread_yield;	// set the signal handler to yield
		sigemptyset(&action.sa_mask);	// Initialize the signal mask to be empty
		action.sa_flags = SA_RESTART;
		if (sigaction(SIGVTALRM, &action, NULL) == -1) {
			perror("Error: cannot handle SIGVTALRM");
			exit(EXIT_FAILURE);
		}
		// Configure a timer that will fire an alarm (through a SIGVTALRM signal) a hundred times per second (i.e. 100 Hz)

  }

}

void preempt_stop(void)
{
/* TODO Phase 4 */
/*
 * preempt_stop - Stop thread preemption
 *
 * Restore previous timer configuration, and previous action associated to
 * virtual alarm signals.
 */

}

