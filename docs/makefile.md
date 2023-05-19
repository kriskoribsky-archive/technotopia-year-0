# Problem sets Makefile

## Targets

- `make build` -> compiles object files and link into final executable named `build.out` inside `build` dir
- `make static` -> builds into `build.out` and runs `cppcheck` static code analysis on all source files
- `make dynamic` -> builds into `build.out` and runs `valgrind` memory analysis tool on final executable
- `make test` -> runs all tests from `tests` directory named either `testh_<name>.c` or `testc_<name>.c`
- `make debug` -> executes the debugger with the compiled executable

Apart from these main ones, some special targets are also available:

- `make full` -> performns full analysis on files

  1. `make build`
  2. `make static`
  3. `make dynamic`
  4. `make test`

- `make clean` -> removes `build` directory created when building
- `make commit` -> cleans project folder and commits changes

## Options

- `w=1` -> Level of warning flag strictness for `compilation`, `static` & `dynamic` testing , default is 0 (no warnings)
- `run=1` -> Run program after compilation, default is 0 (do not run)
- `debug=1` -> Level of debug info printed, default is 0 (silence the commands run)
- `always=1` -> Always build, even if files are up to date
- `w=1` -> Level of warning flags (default is 0)
- `bin=''` -> Specify the name of the generated executable
- `src=''` -> Specify source files to build
- `libs=''` -> Specify additional libraries

## Help

For quick help run `make help`.
