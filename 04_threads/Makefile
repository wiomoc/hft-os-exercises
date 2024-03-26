all: noutofm counter parallel_stats counter_solution_mutex

noutofm: noutofm.c
	gcc -o noutofm noutofm.c -lpthread

counter_solution_mutex: counter_solution_mutex.c
	gcc -o counter_solution_mutex counter_solution_mutex.c -lpthread

counter: counter.c
	gcc -o counter counter.c -lpthread

parallel_stats: parallel_stats.c
	gcc -o parallel_stats parallel_stats.c -lpthread