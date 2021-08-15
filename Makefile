all: format build

format:
	clang-format src/* include/* -i

build:
	mkdir -p build
	cd build && \
	cmake .. && \
	make

debug:
	mkdir -p build
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=debug .. && \
	make

run:
	./build/monitor

clean:
	rm -rf build
