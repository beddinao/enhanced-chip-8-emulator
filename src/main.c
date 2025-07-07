#include <chip8.h>

void _0nnn(chip_8 *chip8) { chip8->pc = chip8->opcode&0xfff; }
void _00e0(chip_8 *chip8) { memset(chip8->display, 0, sizeof(chip8->display)); }
void _00ee(chip_8 *chip8) { if (chip8->sp) chip8->pc = chip8->stack[--chip8->sp]; }
void _1nnn(chip_8 *chip8) { chip8->pc = chip8->opcode&0xfff; }
void _2nnn(chip_8 *chip8) {
	chip8->stack[chip8->sp++] = chip8->pc;
	chip8->pc = chip8->opcode&0xfff;
}
void _3xnn(chip_8 *chip8) { chip8->pc += (chip8->regs[(chip8->opcode&0xf00)>>0x8] == (chip8->opcode&0xff) ? 2 : 0); }
void _4xnn(chip_8 *chip8) { chip8->pc += (chip8->regs[(chip8->opcode&0xf00)>>0x8] != (chip8->opcode&0xff) ? 2 : 0); }
void _5xy0(chip_8 *chip8) { chip8->pc += (chip8->regs[(chip8->opcode&0xf00)>>0x8] == chip8->regs[(chip8->opcode&0xf0)>>0x4] ? 2 : 0); }
void _6xnn(chip_8 *chip8) { chip8->regs[(chip8->opcode&0xf00)>>0x8] = chip8->opcode&0xff; }
void _7xnn(chip_8 *chip8) { chip8->regs[(chip8->opcode&0xf00)>>0x8] += chip8->opcode&0xff; }
void _8xy0(chip_8 *chip8) { chip8->regs[(chip8->opcode&0xf00)>>0x8] = chip8->regs[(chip8->opcode&0xf0)>>0x4]; }
void _8xy1(chip_8 *chip8) { chip8->regs[(chip8->opcode&0xf00)>>0x8] |= chip8->regs[(chip8->opcode&0xf0)>>0x4]; }
void _8xy2(chip_8 *chip8) { chip8->regs[(chip8->opcode&0xf00)>>0x8] &= chip8->regs[(chip8->opcode&0xf0)>>0x4]; }
void _8xy3(chip_8 *chip8) { chip8->regs[(chip8->opcode&0xf00)>>0x8] ^= chip8->regs[(chip8->opcode&0xf0)>>0x4]; }
void _8xy4(chip_8 *chip8) {
	uint16_t res = chip8->regs[(chip8->opcode&0xf00)>>0x8] + chip8->regs[(chip8->opcode&0xf0)>>0x4];
	chip8->regs[(chip8->opcode&0xf00)>>0x8] = res&0xff;
	chip8->regs[0xf] = res > 0xff;
}
void _8xy5(chip_8 *chip8) {
	int16_t res = chip8->regs[(chip8->opcode&0xf00)>>0x8] - chip8->regs[(chip8->opcode&0xf0)>>0x4];
	chip8->regs[(chip8->opcode&0xf00)>>0x8] = res&0xff;
	chip8->regs[0xf] = res >= 0;
}
void _8xy6(chip_8 *chip8) {
	uint8_t old_vx = chip8->regs[(chip8->opcode&0xf00)>>0x8];
	chip8->regs[(chip8->opcode&0xf00)>>0x8] >>= 0x1;
	chip8->regs[0xf] = old_vx&0x1;
}
void _8xy7(chip_8 *chip8) {
	chip8->regs[(chip8->opcode&0xf00)>>0x8] = chip8->regs[(chip8->opcode&0xf0)>>0x4] - chip8->regs[(chip8->opcode&0xf00)>>0x8];
	chip8->regs[0xf] = chip8->regs[(chip8->opcode&0xf0)>>0x4] > chip8->regs[(chip8->opcode&0xf00)>>0x8];
}
void _8xye(chip_8 *chip8) {
	uint8_t tmp_vf = (chip8->regs[(chip8->opcode&0xf00)>>0x8] >> 0x7) & 0x1;
	chip8->regs[(chip8->opcode&0xf00)>>0x8] <<= 0x1;
	chip8->regs[0xf] = tmp_vf;
}
void _9xy0(chip_8 *chip8) { chip8->pc += chip8->regs[(chip8->opcode&0xf00)>>0x8] != chip8->regs[(chip8->opcode&0xf0)>>0x4] ? 2 : 0; }
void _annn(chip_8 *chip8) { chip8->ir = chip8->opcode&0xfff; }
void _bnnn(chip_8 *chip8) { chip8->pc = chip8->regs[0] + (chip8->opcode&0xfff); }
void _cxnn(chip_8 *chip8) { chip8->regs[(chip8->opcode&0xf00)>>0x8] = (rand()%255)&(chip8->opcode&0xff); }
void _dxyn(chip_8 *chip8) {
	unsigned x = chip8->regs[(chip8->opcode&0xf00)>>0x8] % 64;
	unsigned y = chip8->regs[(chip8->opcode&0xf0)>>0x4] % 32;
	uint8_t sprite_pixel, sprite_byte, n = chip8->opcode&0xf;
	uint32_t *display_pixel;
	bool prv;
	chip8->regs[0xf] = 0;
	for (uint8_t row = 0; row < n && row+y < 32; row++) {
		sprite_byte = chip8->ram[chip8->ir+row];
		for (uint8_t col = 0; col < 8&&col+x<64; col++) {
			sprite_pixel = (sprite_byte>>(7-col))&0x1;
			display_pixel = &chip8->display[y+row][x+col];
			/*if (*display_pixel == 1 && sprite_pixel == 1) {
				prv = 1;	
			}*/
			prv = *display_pixel;
			*display_pixel ^= sprite_pixel;
			if (prv && !(*display_pixel)) {
				chip8->regs[0xf] = 1;
			}
		}
	}
}
void _ex9e(chip_8 *chip8) { chip8->pc += (chip8->keyboard[chip8->regs[(chip8->opcode&0xf00)>>0x8]] ? 2 : 0); }
void _exa1(chip_8 *chip8) { chip8->pc += (chip8->keyboard[chip8->regs[(chip8->opcode&0xf00)>>0x8]] ? 0 : 2); }
void _fx07(chip_8 *chip8) { chip8->regs[(chip8->opcode&0xf00)>>0x8] = chip8->delay_timer; }
void _fx0a(chip_8 *chip8) {
	if (chip8->keypress >= 0)
		chip8->regs[(chip8->opcode&0xf00)>>0x8] = chip8->keypress;
	chip8->emu_on = chip8->keypress >= 0;
}
void _fx15(chip_8 *chip8) { chip8->delay_timer = chip8->regs[(chip8->opcode&0xf00)>>0x8]; } 
void _fx18(chip_8 *chip8) { chip8->sound_timer = chip8->regs[(chip8->opcode&0xf00)>>0x8]; }
void _fx1e(chip_8 *chip8) { chip8->ir += chip8->regs[(chip8->opcode&0xf00)>>0x8]; }
void _fx29(chip_8 *chip8) { chip8->ir = chip8->regs[(chip8->opcode&0xf00)>>0x8] * 5 + FONT_START; }
void _fx33(chip_8 *chip8) {
	chip8->ram[chip8->ir] = chip8->regs[(chip8->opcode&0xf00)>>0x8]/100;
	chip8->ram[chip8->ir+1] = (chip8->regs[(chip8->opcode&0xf00)>>0x8]/10)%10;
	chip8->ram[chip8->ir+2] = chip8->regs[(chip8->opcode&0xf00)>>0x8]%10;
}
void _fx55(chip_8 *chip8) { for (uint8_t reg = 0; reg <= (chip8->opcode&0xf00)>>0x8; reg++) chip8->ram[reg+chip8->ir] = chip8->regs[reg]; }
void _fx65(chip_8 *chip8) { for (uint8_t reg = 0; reg <= (chip8->opcode&0xf00)>>0x8; reg++) chip8->regs[reg] = chip8->ram[reg+chip8->ir]; }

void load_instructions(chip_8 *chip8) {
	chip8->_0s_[0] = _0nnn; chip8->_0s_[1] = _00e0; chip8->_0s_[2] = _00ee;
	chip8->_1_7s_[0] = _1nnn; chip8->_1_7s_[1] = _2nnn; chip8->_1_7s_[2] = _3xnn;
	chip8->_1_7s_[3] = _4xnn; chip8->_1_7s_[4] = _5xy0; chip8->_1_7s_[5] = _6xnn;
	chip8->_1_7s_[6] = _7xnn; chip8->_8s_[0] = _8xy0; chip8->_8s_[1] = _8xy1;
	chip8->_8s_[2] = _8xy2;
	chip8->_8s_[3] = _8xy3; chip8->_8s_[4] = _8xy4;
	chip8->_8s_[5] = _8xy5; chip8->_8s_[6] = _8xy6; chip8->_8s_[7] = _8xy7;
	chip8->_8s_[8] = _8xye; chip8->_9_d_[0] = _9xy0; chip8->_9_d_[1] = _annn;
	chip8->_9_d_[2] = _bnnn; chip8->_9_d_[3] = _cxnn; chip8->_9_d_[4] = _dxyn;
	chip8->_es_[0] = _ex9e; chip8->_es_[1] = _exa1; chip8->_fs_[0] = _fx07;
	chip8->_fs_[1] = _fx0a; chip8->_fs_[2] = _fx15; chip8->_fs_[3] = _fx18;
	chip8->_fs_[4] = _fx1e; chip8->_fs_[5] = _fx29; chip8->_fs_[6] = _fx33;
	chip8->_fs_[7] = _fx55; chip8->_fs_[8] = _fx65;
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
	worker->win->win_height = DEF_WIN_HEIGHT;
	worker->win->win_width = DEF_WIN_WIDTH;
	worker->win->ppy = worker->win->win_height/32.0;
	worker->win->ppx = worker->win->win_width/64.0;
	if (!SDL_Init(SDL_INIT_EVENTS)) {
		free(worker->win);
		return false;
	}
	win = SDL_CreateWindow("chip 8 emu",
		worker->win->win_width,
		worker->win->win_height,
		/*SDL_WINDOW_RESIZABLE*/0);
	renderer = SDL_CreateRenderer(win, NULL);
	if (!win || !renderer) {
		if (win) SDL_DestroyWindow(win);
		free(worker->win);
		return false;
	}
	SDL_SetWindowMinimumSize(win,
		MIN_WIN_WIDTH,
		MIN_WIN_HEIGHT);
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
		for (unsigned raw_addr = 0; raw_addr <= chars_read; raw_addr += 0x10) {
			printf("$%04x: ", raw_addr);
			for (unsigned col = 0; col < 0x10&&col + raw_addr < chars_read; col++)
				printf("%02x ", buffer[raw_addr+col]);
			printf("\n");
		}
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
		printf("can't load program\n");
		free(chip8);
		return NULL;
	}
	return chip8;
}

void key_event_handle(worker_data *worker, SDL_Event*event, bool state) {
	bool *keys = worker->chip8->keyboard;
	SDL_Keycode key = event->key.key;

	switch (key) {
		case SDLK_KP_0: key = SDLK_0; break;
		case SDLK_KP_1: key = SDLK_1; break;
		case SDLK_KP_2: key = SDLK_2; break;
		case SDLK_KP_3: key = SDLK_3; break;
		case SDLK_KP_4: key = SDLK_4; break;
		case SDLK_KP_5: key = SDLK_5; break;
		case SDLK_KP_6: key = SDLK_6; break;
		case SDLK_KP_7: key = SDLK_7; break;
		case SDLK_KP_8: key = SDLK_8; break;
		case SDLK_KP_9: key = SDLK_9; break;
		default: break;
	}
	switch (key) {
		case SDLK_0: keys[0x0] = state; worker->chip8->keypress = 0x0; break;
		case SDLK_1: keys[0x1] = state; worker->chip8->keypress = 0x1; break;
		case SDLK_2: keys[0x2] = state; worker->chip8->keypress = 0x2; break;
		case SDLK_3: keys[0x3] = state; worker->chip8->keypress = 0x3; break;
		case SDLK_4: keys[0x4] = state; worker->chip8->keypress = 0x4; break;
		case SDLK_5: keys[0x5] = state; worker->chip8->keypress = 0x5; break;
		case SDLK_6: keys[0x6] = state; worker->chip8->keypress = 0x6; break;
		case SDLK_7: keys[0x7] = state; worker->chip8->keypress = 0x7; break;
		case SDLK_8: keys[0x8] = state; worker->chip8->keypress = 0x8; break;
		case SDLK_9: keys[0x9] = state; worker->chip8->keypress = 0x9; break;
		case SDLK_A: keys[0xa] = state; worker->chip8->keypress = 0xa; break;
		case SDLK_B: keys[0xb] = state; worker->chip8->keypress = 0xb; break;
		case SDLK_C: keys[0xc] = state; worker->chip8->keypress = 0xc; break;
		case SDLK_D: keys[0xd] = state; worker->chip8->keypress = 0xd; break;
		case SDLK_E: keys[0xe] = state; worker->chip8->keypress = 0xe; break;
		case SDLK_F: keys[0xf] = state; worker->chip8->keypress = 0xf; break;
		default: return;
	}
	if (!state)
		worker->chip8->keypress = -1;
}

void draw_bg(win *win, uint32_t color) {
	SDL_SetRenderDrawColor(win->renderer,
		(color>>24)&0xff,
		(color>>16)&0xff,
		(color>>8)&0xff,
		color&0xff);
	SDL_RenderClear(win->renderer);
}

void draw_routine(void *p) {
	struct timespec frame_start_time, frame_end_time;
	uint64_t elapsed_nanoseconds;
	worker_data *worker = (worker_data*)p;
	win *win = worker->win;
	SDL_Event event;
	uint32_t pIndex;
	bool screen_on = true;
	SDL_FPoint points[win->win_height*win->win_width];
	clock_gettime(CLOCK_MONOTONIC, &frame_start_time);
	while (screen_on) {
		pthread_mutex_lock(&worker->halt_mutex);
		if (worker->halt) {
			pthread_mutex_unlock(&worker->halt_mutex);
			return;
		}
		pthread_mutex_unlock(&worker->halt_mutex);
		if (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_EVENT_QUIT: screen_on = false; break;
				case SDL_EVENT_KEY_DOWN: key_event_handle(worker, &event, 1); break;
				case SDL_EVENT_KEY_UP: key_event_handle(worker, &event, 0); break;
				default:	break;
			}
		}
		clock_gettime(CLOCK_MONOTONIC, &frame_end_time);
		elapsed_nanoseconds = (frame_end_time.tv_sec-frame_start_time.tv_sec) * NANOS_PER_SECOND
				+ (frame_end_time.tv_nsec-frame_start_time.tv_nsec);
		if (elapsed_nanoseconds < NANOS_PER_FRAME)
			continue;
		else clock_gettime(CLOCK_MONOTONIC, &frame_start_time);
		pIndex = 0;
		draw_bg(worker->win, 0xfffcf2ff); 
		SDL_SetRenderDrawColor(worker->win->renderer, 0x25, 0x24, 0x22, 0xff);
		for (uint16_t y = 0; y < win->win_height; y++)
			for (uint16_t x = 0; x < win->win_width; x++) 
				if (worker->chip8->display[(uint8_t)(y/win->ppy)][(uint8_t)(x/win->ppx)]) {
					points[pIndex].x = x;
					points[pIndex++].y = y;
				}
		if (pIndex) 
			SDL_RenderPoints(worker->win->renderer, points, pIndex);
		SDL_RenderPresent(worker->win->renderer);
	}
	pthread_mutex_lock(&worker->halt_mutex);
	worker->halt = true;
	pthread_mutex_unlock(&worker->halt_mutex);
}

void *timer_cycle(void *p) {
	struct timespec cycle_start_time, cycle_end_time, sleep_time;
	uint64_t elapsed_nanoseconds;
	worker_data *worker = (worker_data*)p;
	chip_8 *chip8 = worker->chip8;
	memset(&cycle_start_time, 0, sizeof(struct timespec));
	memset(&cycle_end_time, 0, sizeof(struct timespec));
	memset(&sleep_time, 0, sizeof(struct timespec));
	while (true) {
		clock_gettime(CLOCK_MONOTONIC, &cycle_start_time);
		pthread_mutex_lock(&worker->halt_mutex);
		if (worker->halt) {
			pthread_mutex_unlock(&worker->halt_mutex);
			return NULL;
		}
		pthread_mutex_unlock(&worker->halt_mutex);
		if (chip8->delay_timer)
			chip8->delay_timer -= 1;
		if (chip8->sound_timer)
			chip8->sound_timer -= 1;
		clock_gettime(CLOCK_MONOTONIC, &cycle_end_time);
		elapsed_nanoseconds = (cycle_end_time.tv_sec - cycle_start_time.tv_sec)*NANOS_PER_SECOND
			+ (cycle_end_time.tv_nsec-cycle_start_time.tv_nsec);
		if (elapsed_nanoseconds < NANOS_PER_CYCLE) {
			sleep_time.tv_sec = 0;
			sleep_time.tv_nsec = NANOS_PER_CYCLE - elapsed_nanoseconds;
			nanosleep(&sleep_time, NULL);
		}
	}	
}

void *instruction_cycle(void *p) {
	struct timespec frame_start_time, frame_end_time, sleep_time;
	uint64_t elapsed_nanoseconds;
	worker_data *worker = (worker_data*)p;
	chip_8 *chip8 = worker->chip8;
	chip8->emu_on = true;
	uint8_t n;
	memset(&frame_start_time, 0, sizeof(struct timespec));
	memset(&frame_end_time, 0, sizeof(struct timespec));
	memset(&sleep_time, 0, sizeof(struct timespec));
	while (true) {
		clock_gettime(CLOCK_MONOTONIC, &frame_start_time);
		pthread_mutex_lock(&worker->halt_mutex);
		if (worker->halt) {
			pthread_mutex_unlock(&worker->halt_mutex);
			return NULL;	
		}
		pthread_mutex_unlock(&worker->halt_mutex);
		if (chip8->emu_on) {
			if (chip8->pc > PRG_LOAD + chip8->mem_ocu || chip8->pc < PRG_LOAD)
				break;	
			chip8->opcode = chip8->ram[chip8->pc] << 0x8 | chip8->ram[chip8->pc+1];
			chip8->pc += 2;
		}
		switch ((chip8->opcode & 0xf000) >> 12) {
			case 0x0:
				if (chip8->opcode == 0x00e0) chip8->_0s_[1](chip8);
				else if (chip8->opcode == 0x00ee) chip8->_0s_[2](chip8);
				else chip8->_0s_[0](chip8);
				break;
			case 0x8:
				n = chip8->opcode & 0x000f;
				if (n == 0xe) chip8->_8s_[0x8](chip8);
				else chip8->_8s_[n](chip8);
				break;
			case 0xf:
				switch (chip8->opcode & 0xff) {
					case 0x07: chip8->_fs_[0](chip8); break;
					case 0x0a: chip8->_fs_[1](chip8); break;
					case 0x15: chip8->_fs_[2](chip8); break;
					case 0x18: chip8->_fs_[3](chip8); break;
					case 0x1e: chip8->_fs_[4](chip8); break;
					case 0x29: chip8->_fs_[5](chip8); break;
					case 0x33: chip8->_fs_[6](chip8); break;
					case 0x55: chip8->_fs_[7](chip8); break;
					case 0x65: chip8->_fs_[8](chip8); break;
					default:	printf("N(0xf)N ");
				}
				break;
			case 0xe:
				switch (chip8->opcode & 0xff) {
					case 0x9e: chip8->_es_[0](chip8); break;
					case 0xa1: chip8->_es_[1](chip8); break;
					default:	printf("N(0xe)N "); break;
				}
				break;
			default:
				n = (chip8->opcode & 0xf000) >> 12;
				if (n <= 0x7 && n >= 0x1) chip8->_1_7s_[n-0x1](chip8);
				else if (n <= 0xd && n >= 0x9) chip8->_9_d_[n-0x9](chip8);
				else printf("NONE ");
				break;
		}

		clock_gettime(CLOCK_MONOTONIC, &frame_end_time);
		elapsed_nanoseconds = (frame_end_time.tv_sec - frame_start_time.tv_sec) * NANOS_PER_SECOND
			+ (frame_end_time.tv_nsec - frame_start_time.tv_nsec);
		if (elapsed_nanoseconds < NANOS_PER_INSTRUCTION) {
			sleep_time.tv_sec = 0;
			sleep_time.tv_nsec = NANOS_PER_INSTRUCTION - elapsed_nanoseconds;
			nanosleep(&sleep_time, NULL);
		}
	}
	pthread_mutex_lock(&worker->halt_mutex);
	worker->halt = true;
	pthread_mutex_unlock(&worker->halt_mutex);
	return NULL;
}

int main(int c, char **v) {
	if (c != 2) {
		printf("usage: %s [program to execute]\n", v[0]);
		return 1;
	}
	srand(time(NULL));
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
		printf("failed to initiate sdl window\n");
		free(worker);
		free(chip8);
		return 1;
	}
	pthread_mutex_init(&worker->halt_mutex, NULL);
	pthread_mutex_init(&worker->prg_mutex, NULL);
	pthread_mutex_init(&worker->iop_mutex, NULL);
	pthread_create(&worker->worker, NULL, instruction_cycle, worker);
	pthread_create(&worker->clock_worker, NULL, timer_cycle, worker);
	draw_routine(worker);
	pthread_join(worker->worker, NULL);
	pthread_join(worker->clock_worker, NULL);
	pthread_mutex_destroy(&worker->halt_mutex);
	pthread_mutex_destroy(&worker->prg_mutex);
	pthread_mutex_destroy(&worker->iop_mutex);
	SDL_DestroyRenderer(worker->win->renderer);
	SDL_DestroyWindow(worker->win->window);
	SDL_Quit();
	free(worker->win);
	free(worker);
	free(chip8);
}
