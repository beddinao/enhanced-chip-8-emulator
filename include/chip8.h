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

typedef struct graphic {
	SDL_Window *window;
	SDL_Renderer *renderer;
	unsigned win_height;
	unsigned win_width;
	int wpdx;
	int wpdy;
} win;

typedef struct worker {
	pthread_t worker;
	pthread_mutex_t halt_mutex;
	pthread_mutex_t prg_mutex;
	bool halt;
	win *win;
	void *chip8;
} worker_data;

typedef struct chip8 {
	uint8_t ram[RAM_SIZE];
	uint8_t regs[16];
	uint16_t stack[16];
	uint32_t display[64*32];
	uint16_t pc;
	uint16_t ir;
	uint8_t sp;
	uint8_t delay_timer;
	uint8_t sound_timer;
	uint16_t opcode;
	unsigned mem_ocu;
	void (*_0s_[3])(struct chip8*);
	void (*_1_7s_[7])(struct chip8*);
	void (*_8s_[9])(struct chip8*);
	void (*_9_d_[5])(struct chip8*);
	void (*_es_[2])(struct chip8*);
	void (*_fs_[9])(struct chip8*);
} chip_8;

#endif
