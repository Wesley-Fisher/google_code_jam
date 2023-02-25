SHELL:=/bin/bash

CMAKE ?= cmake
TEST ?= tests

make_project_standard: CHECK_PROJECT
	mkdir ${PROJECT}
	cp -ar 00_default_standard/* ${PROJECT}/

do_setup: CHECK_PROJECT
	make clean
	mkdir -p build
	cd build && ${CMAKE} ..

do_compile: CHECK_PROJECT
	cd build && make

do_clean: CHECK_PROJECT
	rm -rf bin
	rm -rf build

run_unit: CHECK_PROJECT
	./bin/tests

run_unitV: CHECK_PROJECT
	./bin/tests_verbose

run_test: CHECK_PROJECT
	cd tests && ../bin/main ${TEST}

run_analyze: CHECK_PROJECT
	cd analysis/sample_test_set_1 && ../../bin/analysis sample_ts1_input sample_ts1_output
	cd analysis/test_set_1 && ../../bin/analysis ts1_input ts1_output
	cd analysis/test_set_2 && ../../bin/analysis ts2_input ts2_output

run_analyzeV: CHECK_PROJECT
	cd analysis/sample_test_set_1 && ../../bin/analysis_verbose sample_ts1_input sample_ts1_output
	cd analysis/test_set_1 && ../../bin/analysis_verbose ts1_input ts1_output
	cd analysis/test_set_2 && ../../bin/analysis_verbose ts2_input ts2_output


.PHONY: build CHECK_PROJECT

CHECK_PROJECT:
ifndef PROJECT
	$(error PROJECT is undefined)
endif