SHELL:=/bin/bash

CMAKE ?= cmake
TEST ?= tests

make_project_standard: CHECK_PROJECT
	mkdir ${PROJECT}
	cp -ar 00_default_standard/* ${PROJECT}/

do_clean: CHECK_PROJECT
	rm -rf  ${PROJECT}/build/CMakeCache.txt

do_setup: CHECK_PROJECT
	make do_clean PROJECT=${PROJECT}
	cd ${PROJECT}/build && ${CMAKE} ../../.. -DMYPROJECT=${PROJECT}

do_compile: CHECK_PROJECT
	cd ${PROJECT}/build && make

run_unit: CHECK_PROJECT
	.${PROJECT}/bin/tests

run_unitV: CHECK_PROJECT
	.${PROJECT}/bin/tests_verbose

run_test: CHECK_PROJECT
	cd ${PROJECT}/tests && ../bin/main ${TEST}

run_analyze: CHECK_PROJECT
	cd ${PROJECT}/analysis/sample_test_set_1 && ../../bin/analysis sample_ts1_input sample_ts1_output
	cd ${PROJECT}/analysis/test_set_1 && ../../bin/analysis ts1_input ts1_output
	cd ${PROJECT}/analysis/test_set_2 && ../../bin/analysis ts2_input ts2_output

run_analyzeV: CHECK_PROJECT
	cd ${PROJECT}/analysis/sample_test_set_1 && ../../bin/analysis_verbose sample_ts1_input sample_ts1_output
	cd ${PROJECT}/analysis/test_set_1 && ../../bin/analysis_verbose ts1_input ts1_output
	cd ${PROJECT}/analysis/test_set_2 && ../../bin/analysis_verbose ts2_input ts2_output


.PHONY: build CHECK_PROJECT

CHECK_PROJECT:
ifndef PROJECT
	$(error PROJECT is undefined)
endif