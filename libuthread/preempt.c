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
#define MICROSEC 10000
struct sigaction signal_action;
struct sigaction old_signal_action;
struct itimerval standard;
struct itimerval old;

bool enable_preempt = false;

void signal_handler()
{
    uthread_yield();
}

/* this func can block signals of type SIGVTALRM */
void preempt_disable(void)
{
    // Disable preemption only if it is currently enabled
    if (enable_preempt)
    {
        enable_preempt = false;
        /* Initialize the signal mask to be empty */
        sigemptyset(&signal_action.sa_mask);

        /* Add the SIGVTALRM signal to the signal set */
        sigaddset(&signal_action.sa_mask, SIGVTALRM);

        /* Block the SIGVTALRM signal in the current thread */
        sigprocmask(SIG_BLOCK, &signal_action.sa_mask, &old_signal_action.sa_mask);
    }
}

/* this func can unblock signals of type SIGVTALRM */
void preempt_enable(void)
{
    // Enable preemption only if it is currently disabled
    if (!enable_preempt)
    {
        enable_preempt = true;
        // Initialize the signal set to be empty
        sigemptyset(&signal_action.sa_mask);

        // Add the SIGVTALRM signal to the signal set
        sigaddset(&signal_action.sa_mask, SIGVTALRM);

        // Unblock the SIGVTALRM signal in the current thread
        sigprocmask(SIG_UNBLOCK, &signal_action.sa_mask, &old_signal_action.sa_mask);
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

    standard.it_value.tv_usec = MICROSEC;
    standard.it_value.tv_sec = 0;
    standard.it_interval.tv_usec = MICROSEC;
    standard.it_interval.tv_sec = 0;
    /* if preempt is enabled */
    if (preempt)
    {
        preempt_enable();
        signal_action.sa_handler = signal_handler;
        sigemptyset(&signal_action.sa_mask);
        sigaddset(&signal_action.sa_mask, SIGVTALRM);
        signal_action.sa_flags = 0;
        sigaction(SIGVTALRM, &signal_action, &old_signal_action);
        setitimer(ITIMER_VIRTUAL, &standard, NULL);
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
    standard = old;
}
