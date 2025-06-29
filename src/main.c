#include <chip8.h>

void _0nnn() { printf("0nnn\n"); }
void _00e0() { printf("00e0\n"); }
void _00ee() { printf("00ee\n"); }

void _1nnn() { printf("1nnn\n"); }
void _2nnn() { printf("2nnn\n"); }
void _3xnn() { printf("3xnn\n"); }
void _4xnn() { printf("4xnn\n"); }
void _5xy0() { printf("5xy0\n"); }
void _6xnn() { printf("6xnn\n"); }
void _7xnn() { printf("7xnn\n"); }

void _8xy0() { printf("8xy0\n"); }
void _8xy1() { printf("8xy1\n"); }
void _8xy2() { printf("8xy2\n"); }
void _8xy3() { printf("8xy3\n"); }
void _8xy4() { printf("8xy4\n"); }
void _8xy5() { printf("8xy5\n"); }
void _8xy6() { printf("8xy6\n"); }
void _8xy7() { printf("8xy7\n"); }
void _8xye() { printf("8xye\n"); }

void _9xy0() { printf("9xy0\n"); }
void _annn() { printf("annn\n"); }
void _bnnn() { printf("bnnn\n"); }
void _cxnn() { printf("cxnn\n"); }
void _dxyn() { printf("dxyn\n"); }

void _ex9e() { printf("ex9e\n"); }
void _exa1() { printf("exa1\n"); }

void _fx07() { printf("fx07\n"); }
void _fx0a() { printf("fx0a\n"); }
void _fx15() { printf("fx15\n"); }
void _fx18() { printf("fx18\n"); }
void _fx1e() { printf("fx1e\n"); }
void _fx29() { printf("fx29\n"); }
void _fx33() { printf("fx33\n"); }
void _fx55() { printf("fx55\n"); }
void _fx65() { printf("fx65\n"); }

void load_instructions(chip_8 *chip8) {
	chip8->_0s_[0] = _0nnn;
	chip8->_0s_[1] = _00e0;
	chip8->_0s_[2] = _00ee;
	chip8->_1_7s_[0] = _1nnn;
	chip8->_1_7s_[1] = _2nnn;
	chip8->_1_7s_[2] = _3xnn;
	chip8->_1_7s_[3] = _4xnn;
	chip8->_1_7s_[4] = _5xy0;
	chip8->_1_7s_[5] = _6xnn;
	chip8->_1_7s_[6] = _7xnn;
	chip8->_8s_[0] = _8xy0;
	chip8->_8s_[1] = _8xy1;
	chip8->_8s_[2] = _8xy2;
	chip8->_8s_[3] = _8xy3;
	chip8->_8s_[4] = _8xy4;
	chip8->_8s_[5] = _8xy5;
	chip8->_8s_[6] = _8xy6;
	chip8->_8s_[7] = _8xy7;
	chip8->_8s_[8] = _8xye;
	chip8->_9_d_[0] = _9xy0;
	chip8->_9_d_[1] = _annn;
	chip8->_9_d_[2] = _bnnn;
	chip8->_9_d_[3] = _cxnn;
	chip8->_9_d_[4] = _dxyn;
	chip8->_es_[0] = _ex9e;
	chip8->_es_[1] = _exa1;
	chip8->_fs_[0] = _fx07;
	chip8->_fs_[1] = _fx0a;
	chip8->_fs_[2] = _fx15;
	chip8->_fs_[3] = _fx18;
	chip8->_fs_[4] = _fx1e;
	chip8->_fs_[5] = _fx29;
	chip8->_fs_[6] = _fx33;
	chip8->_fs_[7] = _fx55;
	chip8->_fs_[8] = _fx65;
}

void load_fonts(chip_8 *chip8) {
	uint8_t fonts[16*5] = {
		0xf0, 0x90, 0x90, 0x90, 0xf0,
		0x20, 0x60, 0x20, 0x20, 0x70,
		0xf0, 0x10, 0xf0, 0x80, 0xf0,
		0xf0, 0x10, 0xf0, 0x10, 0xf0,
		0x90, 0x90, 0xf0, 0x10, 0x10,
		0xf0, 0x80, 0xf0, 0x10, 0xf0,
		0xf0, 0x80, 0xf0, 0x90, 0xf0,
		0xf0, 0x10, 0x20, 0x40, 0x40,
		0xf0, 0x90, 0xf0, 0x90, 0xf0,
		0xf0, 0x90, 0xf0, 0x10, 0xf0,
		0xf0, 0x90, 0xf0, 0x90, 0x90,
		0xe0, 0x90, 0xe0, 0x90, 0xe0,
		0xf0, 0x80, 0x80, 0x80, 0xf0,
		0xe0, 0x90, 0x90, 0x90, 0xe0,
		0xf0, 0x80, 0xf0, 0x80, 0xf0,
		0xf0, 0x80, 0xf0, 0x80, 0x80,
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


bool load_prg(chip_8 *chip8, char *prg) {
	unsigned char buffer[0x1000];
	unsigned chars_read =0;
	FILE *file = fopen(prg, "rb");
	if (!file) return false;
	memset(buffer, 0, sizeof(buffer));
	printf("DUMPING (%s):\n", prg);
	while((chars_read = fread(buffer, 1, sizeof(buffer), file))) {
		if (chars_read + chip8->mem_ocu > RAM_SIZE - PRG_LOAD) {
			fclose(file);
			return false;
		}
		memcpy(chip8->ram + PRG_LOAD + chip8->mem_ocu, buffer, chars_read);
		chip8->mem_ocu += chars_read;
		for (int i = 0; i < chars_read; i++) printf("%02x ", buffer[i]);
		printf("\n");
		memset(buffer, 0, sizeof(buffer));
	}
	fclose(file);
	if (!chip8->mem_ocu)
		return false;
	printf("PROGRAM LOADED\n");
	return true;
}

chip_8 *init_chip8(char *prg) {
	chip_8 *chip8 = malloc(sizeof(chip_8));
	if (!chip8)
		return NULL;
	memset(chip8, 0, sizeof(chip_8));
	load_fonts(chip8);
	load_instructions(chip8);
	chip8->pc = PRG_LOAD;
	if (!load_prg(chip8, prg)) {
		free(chip8);
		return NULL;
	}
	return chip8;
}

void *draw_routine(void *p) {
	worker_data *worker = (worker_data*)p;
	bool screen_on = true;
	SDL_Event event;
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
	bool emu_on = true;
	chip_8 *chip8 = (chip_8*)p;
	worker_data *worker = (worker_data*)p2;
	while (emu_on) {
		pthread_mutex_lock(&worker->halt_mutex);
		if (worker->halt) {
			pthread_mutex_unlock(&worker->halt_mutex);
			return;	
		}
		pthread_mutex_unlock(&worker->halt_mutex);
		if (chip8->pc > PRG_LOAD + chip8->mem_ocu || chip8->pc < PRG_LOAD)
			break;	
		chip8->opcode = chip8->ram[chip8->pc] << 0x8 | chip8->ram[chip8->pc+1];
		chip8->pc += 2;
		printf("%04x ", chip8->opcode);
	}
	pthread_mutex_lock(&worker->halt_mutex);
	worker->halt = true;
	pthread_mutex_unlock(&worker->halt_mutex);
}

int main(int c, char **v) {
	if (c != 2) {
		printf("usage: %s [program to execute]", v[0]);
		return 1;
	}
	chip_8 *chip8 = init_chip8(v[1]);
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
