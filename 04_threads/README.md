# Exercise 04: Threads
The code of all following exercises can be build with `make` 

## Parallel Statistics
Inspect the file `parallel_stats.c`. You see we generate a array of 1,000,000,000 random values and calculate the statistic functions min, max and average of it.
a. Run the programm in the initial state and note the time that the calculation took.
b. To speed up the time required refactor the programm using threads so the statistic operations run in parallel. You will require the methods [pthread_create](https://man7.org/linux/man-pages/man3/pthread_create.3.html) and [pthread_join](https://man7.org/linux/man-pages/man3/pthread_join.3.html).
c. Compare the the time the calculations took before and after the refactoring. What speed-up did we get?
d. How could you hypotheticly speed up the calculation even more, assuming you have more than three cores / threads?


## Counter
Inpect the file `counter.c`. We use multiple threads to count up to `N * STEPS_PER_THREAD`. In practice, this can happen, for example, when several threads are used to search for the occurrence of a search string in a long text.
a. Run the programm and try to reason why the actual value doesn't match the expected value. Also measure the time used with the program [time](https://man7.org/linux/man-pages/man1/time.1.html)
b. Correct the program using mutexes so that actual == expected. You will require the methods [pthread_mutex_init](https://man7.org/linux/man-pages/man3/pthread_mutex_init.3p.html), [pthread_mutex_lock](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html), [pthread_mutex_unlock](https://man7.org/linux/man-pages/man3/pthread_mutex_unlock.3p.html). Measure the time required and reason about the time difference.
c. Correct the program using atomics so that actual == expected. You will require the method [atomic_fetch_add](https://en.cppreference.com/w/c/atomic/atomic_fetch_add).

## N-out-of-M
Inspect the file `noutofm.c`. We starting `M` thread, each thread sleep for an indiviual time between 1-10 seconds and after every thread finished that we terminate.
Now change the program in the way, that the program terminates after only `N` threads are finished with their sleep. Use [condition variables](https://stackoverflow.com/a/20772586/5048815) in the implementation.

Methods required: `pthread_create`, `pthread_mutex_init`, `pthread_cond_init`, `pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`, `pthread_cond_signal`, `pthread_cond_wait`.

## N-out-of-M in Java
