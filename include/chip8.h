#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <SDL3/SDL.h>

#define DEF_WIN_WIDTH 1000
#define DEF_WIN_HEIGHT 500
#define MIN_WIN_WIDTH 200
#define MIN_WIN_HEIGHT 100
#define RAM_SIZE 0x1000 
#define FONT_START 0x50
#define PRG_LOAD 0x200
#define NANOS_PER_SECOND 1000000000

#define INSTRUCTIONS_PER_SECOND 700 
#define NANOS_PER_INSTRUCTION NANOS_PER_SECOND/INSTRUCTIONS_PER_SECOND

#define CYCLES_PER_SECOND 60 // 60hz
#define NANOS_PER_CYCLE NANOS_PER_SECOND/CYCLES_PER_SECOND

#define FPS 60
#define NANOS_PER_FRAME NANOS_PER_SECOND/FPS

typedef struct graphic {
	SDL_Window *window;
	SDL_Renderer *renderer;
	uint16_t win_height;
	uint16_t win_width;
	float ppx;
	float ppy;
} win;

typedef struct io_p {
	bool display[32][64];
	bool keyboard[16];
	int8_t keypress;
} io_p;

typedef struct chip8 {
	uint8_t ram[RAM_SIZE];
	uint16_t pc;
	uint16_t ir;
	uint8_t sp;
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint16_t opcode;
	uint8_t regs[16];
	uint16_t stack[16];
	void (*_0s_[3])(struct chip8*);
	void (*_1_7s_[7])(struct chip8*);
	void (*_8s_[9])(struct chip8*);
	void (*_9_d_[5])(struct chip8*);
	void (*_es_[2])(struct chip8*);
	void (*_fs_[9])(struct chip8*);
	unsigned mem_ocu;
	bool emu_on;
} chip_8;

typedef struct worker {
	bool halt;
	pthread_t worker;
	pthread_t clock_worker;
	pthread_mutex_t halt_mutex;
	pthread_mutex_t iop_mutex;
	pthread_mutex_t prg_mutex;
	chip_8 *chip8;
	io_p *iop;
	win *win;
} worker_data;

#endif
