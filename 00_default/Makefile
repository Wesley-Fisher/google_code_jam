SHELL:=/bin/bash

build:
ifndef CMAKE
	echo "Specify CMake"
	exit()
endif
	mkdir -p build
	cd build && ${CMAKE} ..
	cd build && make

test:
	./bin/tests

run:
	./bin/main

.PHONY: build
