all: 
	@cmake -B build
	@cmake --build build
	@mv build/c8emu .

clean:
	rm -rf build c8emu

