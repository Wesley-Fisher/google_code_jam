SHELL:=/bin/bash

CMAKE ?= cmake

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
	echo "Enter testing script here"

.PHONY: build
