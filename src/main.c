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
	memcpy(chip8->ram + FONT_START, fonts, sizeof(fonts));
}

bool init_window(worker_data *worker) {
	worker->win = malloc(sizeof(win));
	if (!worker->win)
		return false;
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	if (!SDL_Init(SDL_INIT_EVENTS)) {
		free(worker->win);
		return false;
	}
	win = SDL_CreateWindow("chip 8 emu", DEF_WIN_WIDTH, DEF_WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(win, NULL);
	if (!win || !renderer) {
		if (win) SDL_DestroyWindow(win);
		free(worker->win);
		return false;
	}
	SDL_SetWindowMinimumSize(win, MIN_WIN_WIDTH, MIN_WIN_HEIGHT);
	worker->win->window = win;
	worker->win->renderer = renderer;
	return true;
}

void clean_sdl(worker_data *worker) {
	SDL_DestroyWindow(worker->win->window);
	SDL_DestroyRenderer(worker->win->renderer);
	SDL_Quit();	
}

chip_8 *init_chip8() {
	chip_8 *chip8 = malloc(sizeof(chip_8));
	if (!chip8)
		return NULL;
	memset(chip8, 0, sizeof(chip_8));
	load_fonts(chip8);
	chip8->pc = PRG_LOAD;
	return chip8;
}

void *draw_routine(void *p) {
	worker_data *worker = (worker_data*)p;
	SDL_Event event;
	bool screen_on = true;
	while (screen_on) {
		pthread_mutex_lock(&worker->halt_mutex);
		if (worker->halt) {
			pthread_mutex_unlock(&worker->halt_mutex);
			return NULL;
		}
		pthread_mutex_unlock(&worker->halt_mutex);
		if (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_EVENT_QUIT:
					screen_on = false;
					break;
				case SDL_EVENT_KEY_DOWN:
					if (event.key.key == SDLK_ESCAPE)
						screen_on = false;
					break;
				default:	break;
			}
		}
	}
	pthread_mutex_lock(&worker->halt_mutex);
	worker->halt = true;
	pthread_mutex_unlock(&worker->halt_mutex);
	return NULL;
}

void instruction_cycle(void *p, void *p2) {
	chip_8 *chip8 = (chip_8*)p;
	worker_data *worker = (worker_data*)p2;
	while (true) {
		pthread_mutex_lock(&worker->halt_mutex);
		if (worker->halt) {
			pthread_mutex_unlock(&worker->halt_mutex);
			break;
		}
		pthread_mutex_unlock(&worker->halt_mutex);
	}
}

int main(int c, char **v) {
	chip_8 *chip8 = init_chip8();
	if (!chip8)
		return 1;
	worker_data *worker = malloc(sizeof(worker_data));
	if (!worker) {
		free(chip8);
		return 1;
	}
	memset(worker, 0, sizeof(worker_data));
	worker->chip8 = chip8;
	if (!init_window(worker)) {
		free(worker);
		free(chip8);
		return 1;
	}
	pthread_mutex_init(&worker->halt_mutex, NULL);
	pthread_mutex_init(&worker->prg_mutex, NULL);
	pthread_create(&worker->worker, NULL, draw_routine, worker);
	instruction_cycle(chip8, worker);
	//
	pthread_join(worker->worker, NULL);
	pthread_mutex_destroy(&worker->halt_mutex);
	pthread_mutex_destroy(&worker->prg_mutex);
	SDL_DestroyRenderer(worker->win->renderer);
	SDL_DestroyWindow(worker->win->window);
	SDL_Quit();
	free(worker->win);
	free(worker);
	free(chip8);
}
