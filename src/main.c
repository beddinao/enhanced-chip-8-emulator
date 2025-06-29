#include <chip8.h>


int main() {
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	if (!SDL_Init(SDL_INIT_EVENTS))
		return 1;
	win = SDL_CreateWindow("chip 8 emu", DEF_WIN_WIDTH, DEF_WIN_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!win || !(renderer = SDL_CreateRenderer(win, NULL))) {
		if (win) SDL_DestroyWindow(win);
		return 1;
	}
	SDL_SetWindowMinimumSize(win, MIN_WIN_WIDTH, MIN_WIN_HEIGHT);
	sleep(3);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}
