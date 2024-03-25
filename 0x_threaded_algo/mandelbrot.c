#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <pthread.h>
#include <cgif.h>
#include <math.h>

#define FRAMES 20
#define HEIGHT 1000
#define WIDTH 1000
#define MAX_ITER 1000
#define PALLETE_SIZE 256
#define COMPONENTS_PER_COLOR 3

#define SCALE(x, old_min, old_max, new_min, new_max) (x - old_min) * ((new_max - new_min) / (old_max - old_min)) + new_min

uint8_t mandelbrot_pixel(int x, int y) {
    double x_scaled =  SCALE(x, 0.0, WIDTH - 1, -1.5, 1.5); 
    double y_scaled =  SCALE(y, 0.0, HEIGHT - 1, -1.5, 1.5);

    double u = 0; 
    double v = 0;

    int iteration;
    for(iteration = 0; u * u + v * v <= 4.0 && iteration < MAX_ITER; iteration++) {
        double temp = u * u - v * v + x_scaled;
        v = 2 * u * v + y_scaled;
        u = temp;
    }
    return iteration / (MAX_ITER / PALLETE_SIZE);
}

void hsv2rgb(float H, float S, float V, uint8_t* out) {
	float r, g, b;
	
	float h = H / 360;
	float s = S / 100;
	float v = V / 100;
	
	int i = floor(h * 6);
	float f = h * 6 - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	
	switch (i % 6) {
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
	}
	
	out[0] = r * 255;
	out[1] = g * 255;
	out[2] = b * 255;
}

void generate_palatte(uint8_t* palette) {
    for(int i = 0; i < PALLETE_SIZE; i++) {
        hsv2rgb(i * (360.0 / PALLETE_SIZE), 100, (i < PALLETE_SIZE - 1) ? 100: 0, &palette[i * COMPONENTS_PER_COLOR]);
    }
}

int main(int argc, char**  argv) {
    CGIF_Config gif_config;
    memset(&gif_config, 0, sizeof(CGIF_Config));
    uint8_t pallete[PALLETE_SIZE * COMPONENTS_PER_COLOR];
    generate_palatte(pallete);
    gif_config.path = "mandelbrot.gif";
    gif_config.attrFlags = CGIF_ATTR_IS_ANIMATED;
    gif_config.width = WIDTH;
    gif_config.height = HEIGHT;
    gif_config.numGlobalPaletteEntries = PALLETE_SIZE;
    gif_config.pGlobalPalette = pallete;

    CGIF* gif = cgif_newgif(&gif_config);

    uint8_t* frame_pixels = malloc(WIDTH * HEIGHT * sizeof(uint8_t));

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            frame_pixels[x * HEIGHT + y] = mandelbrot_pixel(x, y);
        }
    }

    CGIF_FrameConfig gif_frame_config;
    memset(&gif_frame_config, 0, sizeof(CGIF_FrameConfig));
    gif_frame_config.pImageData = frame_pixels;
    cgif_addframe(gif, &gif_frame_config);

    cgif_close(gif);
}