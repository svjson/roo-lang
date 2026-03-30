build:
	cmake -B build
	cmake --build build

install:
	cmake --build build --target install
