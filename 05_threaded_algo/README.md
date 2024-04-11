# Exercise 05: Threaded Algorithms & High Level Abstractions in Java
## Parallel Monte Carlo
The [Monte Carlo method](https://en.wikipedia.org/wiki/Monte_Carlo_method) is a algorithm to approixmate PI. Parralize the code in `monte_carlo.c` using threads for speed up.

## Parallel Mandelbrot
[Mandelbrot sets](https://en.wikipedia.org/wiki/Mandelbrot_set) are fractals that could be used for wonderful animations. Parralize the code in `mandelbrot.c` using threads for speed up.

## Parallel Sorts
The threadsort program creates an array with random numbers and sorts it in various ways. The auxiliary function `timeSort` copies the array (so that each algorithm starts with the same basis), calls the corresponding algorithm and measures the times required for sorting. The functions `Arrays::sort` and `Arrays::parallelSort` are already implemented by Java.

Implement 3 missing approaches:
- `multiThreadedSort`: divide the array into `n_threads` subranges, each of which is sorted in its own thread. The subranges are then merged (brought into a common order)
- `forkJoinSortFull`: Build a `RecursiveTask` and execute it via `ForkJoinPool`. Threshold for direct processing is 2, i.e. only when only 2 values remain are they sorted in the thread. Otherwise the array is split, sorted separately and then merged.
- `forkJoinSort1000`: Also with `ForkJoinPool` and `RecursiveTask` but this time the threshold is 1000. For sorting below the threshold, you can use Arrays::sort, for example.