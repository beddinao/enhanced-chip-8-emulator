#include <chip8.h>

void load_fonts(chip_8 *chip8) {
	uint8_t fonts[16*5] = {
		0xf0, 0x90, 0x90, 0x90, 0xf0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xf0, 0x10, 0xf0, 0x80, 0xf0, // 2
		0xf0, 0x10, 0xf0, 0x10, 0xf0, // 3
		0x90, 0x90, 0xf0, 0x10, 0x10, // 4
		0xf0, 0x80, 0xf0, 0x10, 0xf0, // 5
		0xf0, 0x80, 0xf0, 0x90, 0xf0, // 6
		0xf0, 0x10, 0x20, 0x40, 0x40, // 7
		0xf0, 0x90, 0xf0, 0x90, 0xf0, // 8
		0xf0, 0x90, 0xf0, 0x10, 0xf0, // 9
		0xf0, 0x90, 0xf0, 0x90, 0x90, // a
		0xe0, 0x90, 0xe0, 0x90, 0xe0, // b
		0xf0, 0x80, 0x80, 0x80, 0xf0, // c
		0xe0, 0x90, 0x90, 0x90, 0xe0, // d
		0xf0, 0x80, 0xf0, 0x80, 0xf0, // e
		0xf0, 0x80, 0xf0, 0x80, 0x80, // f
	};
	memcpy(chip8->ram + RAM_START, fonts, sizeof(fonts));
}

bool init_window(chip_8 *chip8) {
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	if (!SDL_Init(SDL_INIT_EVENTS))
		return false;
	win = SDL_CreateWindow("chip 8 emu", DEF_WIN_WIDTH, DEF_WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!win || !(renderer = SDL_CreateRenderer(win, NULL))) {
		if (win) SDL_DestroyWindow(win);
		return false;
	}
	SDL_SetWindowMinimumSize(win, MIN_WIN_WIDTH, MIN_WIN_HEIGHT);
	chip8->win = win;
	chip8->renderer = renderer;
	return true;
}

void clean_sdl(chip_8 *chip8) {
	SDL_DestroyWindow(chip8->win);
	SDL_DestroyRenderer(chip8->renderer);
	SDL_Quit();	
}

void *draw_routine(void *p) {
	worker_data *worker = (worker_data*)p;
	while (true) {
		pthread_mutex_lock(&worker->halt_mutex);
		if (worker->halt) {
			pthread_mutex_unlock(&worker->halt_mutex);
			break;
		}
		pthread_mutex_unlock(&worker->halt_mutex);
	}
	return NULL;
}

int main() {
	chip_8 *chip8 = malloc(sizeof(chip_8));
	if (!chip8)
		return 1;
	memset(chip8, 0, sizeof(chip_8));
	if (!init_window(chip8)) {
		free(chip8);
		return 1;
	}
	worker_data *worker = malloc(sizeof(worker_data));
	if (!worker) {
		clean_sdl(chip8);
		free(chip8);
		return 1;
	}
	memset(worker, 0, sizeof(worker_data));
	pthread_mutex_init(&worker->halt_mutex);
	pthread_mutex_init(&worker->prg_mutex);
	pthread_create(&worker->worker, NULL, draw_routine, worker);
	instruction_cycle(chip8);
	pthread_mutex_lock(&worker->halt_mutex);
	worker->halt = true;
	pthread_mutex_unlock(&worker->halt_mutex);
	pthread_join(worker->worker, NULL);
	SDL_DestroyRenderer(chip8->renderer);
	SDL_DestroyWindow(chip8->win);
	SDL_Quit();
	free(worker);
	free(chip8);
}
