all: 
	@cmake -B build
	@cmake --build build
	@mv build/c8emu .

clean:
	rm -fr build c8emu
