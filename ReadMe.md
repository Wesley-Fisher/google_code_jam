# Google Code Jam Solutions - Wesley Fisher

This is a project where I am working on developing a framework for participating in Google Code Jam. I am working on setting this up to include:

* Standard shared functions for questions/problems
* Unit-tests for better development
* Logging functions/macros
* Code that can be submitted for Google Code Jam, as well as run with logging and tests locally, with only changes at the compile stage
* A single `make`-based interface for running everything at the top level

This ReadMe serves as some of my own documentation.

To date, development on this has been more haphazard on this than I would like (ex: commits un-doing work from prevous commits). I hope to deal with this better in the future.

# Commands

All `make` commands require an argument in the form of `PROJECT=<Path>`, where `<Path>` should be in the form of `<year>/<round>/<question>`.

`init_dependencies`: Installs for Ubuntu. Admittedly this could be better moved somewhere else, but is not a priority to improve.

`make_project_standard`: create a folder for the project, creates standard subfolders, and copies the template `problem.cpp` file.

`do_clean`: removes the CMake cache of a project.

`do_setup`: calls CMake for a project.

`do_compile`: compiles executables for a project.

`run_unit`: runs unit-tests for a project.

`run_unitV`: runs unit-tests with verbose logging.

`run_test`: run executable of a project on a given file.

`run_analyze`: run analysis for a project.

`run_analyzeV`: run analysis for a project with verbose logging.

`make_submission`: copy project's `problem.cpp` file into a `submission` subdirectory, and replace the inclusion of `main_standard.h` with the entire contents of the file.
