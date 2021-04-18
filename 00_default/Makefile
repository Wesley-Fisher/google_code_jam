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

unit:
	./bin/tests

test:
ifndef TEST
	TEST=tests
endif
	./bin/main ${TEST}

.PHONY: build
