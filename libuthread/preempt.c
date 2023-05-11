#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "private.h"
#include "uthread.h"


/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100
#define MICROSEC 10000
struct sigaction signal_action;
struct sigaction old_signal_action;
struct itimerval standard;
struct itimerval old;

bool enable_preempt = false;

void signal_handler(){
    uthread_yield();
}

/* this func can block signals of type SIGVTALRM */
void preempt_disable(void)
{
	/* Create a signal set to hold the mask of blocked signals */

    /* Initialize the signal mask to be empty */
    sigemptyset(&signal_action.sa_mask);

    /* Add the SIGVTALRM signal to the signal set */
    sigaddset(&signal_action.sa_mask, SIGVTALRM);

    /* Block the SIGVTALRM signal in the current thread */
    sigprocmask(SIG_BLOCK, &signal_action.sa_mask, &old_signal_action.sa_mask);

    // Disable preemption only if it is currently enabled
    if (enable_preempt) {
        enable_preempt = false;
    }
}

/* this func can unblock signals of type SIGVTALRM */
void preempt_enable(void)
{
	// Create a signal set to hold the mask of unblocked signals


    // Initialize the signal set to be empty
    sigemptyset(&signal_action.sa_mask);

    // Add the SIGVTALRM signal to the signal set
    sigaddset(&signal_action.sa_mask, SIGVTALRM);

    // Unblock the SIGVTALRM signal in the current thread
    sigprocmask(SIG_UNBLOCK, &signal_action.sa_mask, &old_signal_action.sa_mask);

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
        preempt_enable();
        signal_action.sa_handler = signal_handler;
        sigaction(SIGALRM, &signal_action, &old_signal_action);
        ualarm(MICROSEC, 0);
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
    // getitimer(ITIMER_VIRTUAL, );
    preempt_disable();
    signal_action = old_signal_action;
}

