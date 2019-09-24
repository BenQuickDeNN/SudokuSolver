# set compiler
CC="g++"

# set source code
src="src/sudoku_solver.cpp"

# set include directory
include="include"

# set build directory
build="build"

# set binary directory
bin="bin/sudoku_solver"

# set standard
CppSTD="c++17"

# quick building
$CC $src -o $bin -I $include -g -std=$CppSTD