#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <SDL3/SDL.h>

#define DEF_WIN_WIDTH 1024
#define DEF_WIN_HEIGHT 512
#define MIN_WIN_WIDTH 200
#define MIN_WIN_HEIGHT 100
#define RAM_SIZE 0x1000 
#define FONT_START 0x50
#define PRG_LOAD 0x200
#define NANOS_PER_SECOND 1000000000
#define BGC 0xfffcf2ff
#define FGC 0x252422ff
#define INSTRUCTIONS_PER_SECOND 700 
#define NANOS_PER_INSTRUCTION NANOS_PER_SECOND/INSTRUCTIONS_PER_SECOND
#define CYCLES_PER_SECOND 60 //hz
#define NANOS_PER_CYCLE NANOS_PER_SECOND/CYCLES_PER_SECOND
#define FPS 60
#define NANOS_PER_FRAME NANOS_PER_SECOND/FPS

typedef struct graphic {
	SDL_Window	*window;
	SDL_Renderer	*renderer;
	uint16_t	win_height;
	uint16_t	win_width;
	uint8_t	ppx;
	uint8_t	ppy;
} win;

typedef struct chip8 {
	uint16_t	pc;
	uint16_t	ir;
	uint8_t	sp;
	uint8_t	delay_timer;
	uint8_t	sound_timer;

	uint8_t	ram[RAM_SIZE];
	uint8_t	display[32][64];
	bool	keyboard[16];
	uint8_t	regs[16];
	uint16_t	stack[16];

	void	(*_0s_[3])(struct chip8*);
	void	(*_1_7s_[7])(struct chip8*);
	void	(*_8s_[9])(struct chip8*);
	void	(*_9_d_[5])(struct chip8*);
	void	(*_es_[2])(struct chip8*);
	void	(*_fs_[9])(struct chip8*);

	uint16_t	opcode;
	uint16_t	mem_ocu;
	int8_t	keypress;
	bool	emu_on;
} chip_8;

typedef struct worker {
	bool	halt;
	pthread_t	worker;
	pthread_t	clock_worker;
	pthread_mutex_t	halt_mutex;
	pthread_mutex_t	iop_mutex;
	pthread_mutex_t	prg_mutex;
	win	*win;
	chip_8	*chip8;
} worker_data;

#endif
