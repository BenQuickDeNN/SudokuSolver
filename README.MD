A project developed by Bin Qu to automatically solves Sudoku.

# Algorithm
## Main Structure
```mermaid
graph TD
    START("start")-->LOAD["load files"]
    LOAD-->INIT["initialize gridsize n*n and blocksize m*m"]
    INIT-->NOB["compute the number of blocks nb=n/m"]
    NOB-->GC["generate candidates"]
    GC-->FILL1["fill lattices"]
    FILL1-->INII["initialize i=1"]
    INII-->EXCLUDING["i-excluding"]
    EXCLUDING-->IF1{"any update?"}
    IF1--yes-->FILL2["fill lattices"]
    IF1--no-->PLUSI["i++"]
    PLUSI-->BI{"i<=nb?"}
    BI--yes-->EXCLUDING
    BI--no-->EXM["try exhaustive method"]
    FILL2-->ALF{all filled?}
    ALF--yes-->OUTPUT
    ALF--no-->RESETI["reset i=1"]
    RESETI-->EXCLUDING
    OUTPUT-->END("end")
    EXM-->END
```
## Excluding

# Sudoku Game File Structure (Deprecated)
## Example
```
# no. 001 sudoku game
grid_length = 9
block_length = 3
grid =
[
    7,9,2,0,0,5,4,0,0;
    0,0,8,0,2,0,0,6,0;
    0,5,4,3,9,8,0,0,0;
    2,0,5,7,0,0,0,0,3;
    8,0,3,1,0,0,5,0,0;
    1,7,0,5,3,4,8,2,6;
    0,3,0,9,0,1,0,8,0;
    0,8,7,0,0,0,0,0,0;
    0,0,0,0,4,6,3,0,0
]
```
note: '0' in grid matrix represents "empty".
## FSM of Sudoku Game Script Language
```mermaid
graph LR
    START--'#'-->S1
    START--letter-->S2
    START--space/enter-->S3
    START--digit-->S4
    START--'='-->S5
    START--lb-->S6
    START--rb-->S7
    START--EOF-->EMPTY

    S1--not enter/not EOF-->S1
    S1--enter/EOF-->EMPTY

    S2--letter/digit/underline-->S2
    S2--'#'/'='/semi/space/enter/EOF-->VAR

    S3--letter-->S2
    S3--space/enter-->S3
    S3--digit-->S4
    S3--'='-->S5
    S3--EOF-->EMPTY

    S4--digit-->S4
    S4--'#'/','/semi/space/enter/EOF-->DIGIT

    S5--space/digit/enter/'#'/lb-->EQUAL

    S6--space/digit/enter/'#'/-->LB

    S7--space/enter/'#'/EOF-->RB
```
notes:
1. "LB" represents '[', and "RB" represents ']'.
2. "semi" represent ';'.
3. S2, S4, S5, S6, S7 collects char elements.