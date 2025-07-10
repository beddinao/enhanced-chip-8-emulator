#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <SDL3/SDL.h>

#define RAM_SIZE 0x1000 
#define FONT_START 0x50
#define PRG_LOAD 0x200
#define NANOS_PER_SECOND 0x3B9ACA00
#define BGC 0xfffcf2ff
#define FGC 0x252422ff
#define INSTRUCTIONS_PER_SECOND 0x2bc//700 
#define NANOS_PER_INSTRUCTION NANOS_PER_SECOND/INSTRUCTIONS_PER_SECOND
#define CYCLES_PER_SECOND 0x3c//60hz
#define NANOS_PER_CYCLE NANOS_PER_SECOND/CYCLES_PER_SECOND
#define FPS 60
#define NANOS_PER_FRAME NANOS_PER_SECOND/FPS
#define DEF_WIN_WIDTH 0x500 
#define DEF_WIN_HEIGHT 0x280 
#define MIN_WIN_WIDTH 0xc8
#define MIN_WIN_HEIGHT 0x64

typedef struct graphic {
	SDL_Window	*window;
	SDL_Renderer	*renderer;
	Uint16	win_height;
	Uint16	win_width;
	Uint8	ppx;
	Uint8	ppy;
} win;

typedef struct chip8 {
	Uint16	pc;
	Uint16	ir;
	Uint8	sp;
	Uint8	delay_timer;
	Uint8	sound_timer;

	Uint8	ram[RAM_SIZE];
	Uint8	display[32][64];
	bool	keyboard[16];
	Uint8	regs[16];
	Uint16	stack[16];

	void	(*_0s_[3])(struct chip8*);
	void	(*_1_7s_[7])(struct chip8*);
	void	(*_8s_[9])(struct chip8*);
	void	(*_9_d_[5])(struct chip8*);
	void	(*_es_[2])(struct chip8*);
	void	(*_fs_[9])(struct chip8*);

	Uint16	opcode;
	Uint16	mem_ocu;
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
