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

unitV:
	./bin/tests_verbose

test:
	cd tests && ../bin/main ${TEST}

analyze:
	cd analysis/sample_test_set_1 && ../../bin/analysis sample_ts1_input sample_ts1_output
	cd analysis/test_set_1 && ../../bin/analysis ts1_input ts1_output
	cd analysis/test_set_2 && ../../bin/analysis ts2_input ts2_output

analyzeV:
	cd analysis/sample_test_set_1 && ../../bin/analysis_verbose sample_ts1_input sample_ts1_output
	cd analysis/test_set_1 && ../../bin/analysis_verbose ts1_input ts1_output
	cd analysis/test_set_2 && ../../bin/analysis_verbose ts2_input ts2_output


.PHONY: build
