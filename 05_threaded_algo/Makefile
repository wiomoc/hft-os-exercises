all: monte_carlo mandelbrot

monte_carlo: monte_carlo.c
	gcc -o monte_carlo monte_carlo.c -lpthread

mandelbrot: mandelbrot.c
	gcc -Icgif/inc/ -o mandelbrot mandelbrot.c cgif/src/cgif.c cgif/src/cgif_raw.c -lm -lpthread