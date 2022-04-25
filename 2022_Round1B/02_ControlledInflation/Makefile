SHELL:=/bin/bash

CMAKE ?= cmake
TEST ?= tests

build:
	make clean
	mkdir -p build
	cd build && ${CMAKE} ..
	

compile:
	cd build && make

clean:
	rm -rf bin
	rm -rf build

unit:
	./bin/tests

test:
	cd bin && ./main ${TEST}

.PHONY: build
