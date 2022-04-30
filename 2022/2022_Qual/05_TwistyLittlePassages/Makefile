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
	cd tests && python interactive_runner.py python3 local_testing_tool.py3 0 -- ../bin/main

.PHONY: build
