#include <string>

std::string CLI_HELP =
"\
Usage: sudoku_solver(.exe) [options] file...\r\n\
Options:\r\n\
  --help(-h)              Display this information.\r\n\
  --version(-v)           Display the version.\r\n\
  --file(-f) <file>       Process a sudoku game file.\r\n\
For more information, please see:\r\n\
<https://github.com/BenQuickDeNN/SudokuSolver>.\r\n\
"
;

std::string CLI_VERSION =
"\
Sodoku Solver. Version alpha 1.0\r\n\
"
;

std::string CLI_INVALID_INFO = 
"\
Invalid command. For more information please type \"--help(-h)\"\r\n\
"
;