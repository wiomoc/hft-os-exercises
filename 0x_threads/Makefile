all: noutofm counter parallel_stats counter_solution_mutex

noutofm: noutofm.c
	gcc -o noutofm -lpthread noutofm.c

counter_solution_mutex: counter_solution_mutex.c
	gcc -o counter_solution_mutex counter_solution_mutex.c

counter: counter.c
	gcc -o counter counter.c

parallel_stats: parallel_stats.c
	gcc -o parallel_stats parallel_stats.c