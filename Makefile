SHELL:=/bin/bash

CMAKE ?= cmake
TEST ?= tests

init_dependencies:
	sudo apt install libboost-all-dev
	sudo apt install build-essential

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
	./${PROJECT}/bin/tests

run_unitV: CHECK_PROJECT
	./${PROJECT}/bin/tests_verbose

run_test: CHECK_PROJECT
	cd ${PROJECT}/tests && ../bin/main ${TEST}

run_analyze: CHECK_PROJECT
	cd ${PROJECT}/analysis && ../bin/analysis sample_ts1_input sample_ts1_output
	cd ${PROJECT}/analysis && ../bin/analysis ts1_input ts1_output
	cd ${PROJECT}/analysis && ../bin/analysis ts2_input ts2_output

run_analyzeV: CHECK_PROJECT
	cd ${PROJECT}/analysis && ../bin/analysis_verbose sample_ts1_input sample_ts1_output
	cd ${PROJECT}/analysis && ../bin/analysis_verbose ts1_input ts1_output
	cd ${PROJECT}/analysis && ../bin/analysis_verbose ts2_input ts2_output

make_submission: CHECK_PROJECT
	cp ${PROJECT}/problem.cpp ${PROJECT}/submission/problem.cpp
	sed -n '/#include \"main_standard.h\"/!{p;d;}; r src/main_standard.h' ${PROJECT}/submission/problem.cpp > ${PROJECT}/submission/problem_out.cpp
	rm ${PROJECT}/submission/problem.cpp


.PHONY: build CHECK_PROJECT

CHECK_PROJECT:
ifndef PROJECT
	$(error PROJECT is undefined)
endif