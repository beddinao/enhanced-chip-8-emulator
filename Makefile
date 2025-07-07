all: 
	cmake -B build -DCMAKE_CXX_FLAGS="-fsanitize=address -g"
	cmake --build build
	mv build/c8emu .

clean:
	rm -rf build c8emu

