SHELL:=/bin/bash

build:
ifndef CMAKE
	echo "Specify CMake"
	exit()
endif
	make clean
	mkdir -p build
	cd build && ${CMAKE} ..
	

compile:
	cd build && make

clean:
	rm -rf bin
	rm -rf build

test:
	./bin/tests

run:
	./bin/main

.PHONY: build
