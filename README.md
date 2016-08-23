# Dagan's Brainfuck Interpreter
DBFI is a normal Brainfuck interpreter bassed off the [Hyper-Dimensional Brainfuck](https://github.com/Property404/hdbf) interpreter(hdbf). I use to use hdbf for all my brainfucking needs, but I felt the need to have a normal implementation.

## Command line options:
`-d`	Allow debugging commands (`#`)  
`-h` or `--help`	Print help message
`-i cmd`	Pass argument as code instead of filename
`-o`	Optimize code before running  
`-t`	Display run time of program  
`-v`	Display DBFI version number

## Installing
    git clone https://github.com/Property404/dbfi
    cd dbfi
    make;sudo make install
    # test
    bf examples/LostKingdom.bf

## Possible future features:  
[Trivial Brainfuck Substition](https://esolangs.org/wiki/TrivialBrainfuckSubstitution) interpretation  
Forbidding wrapping  
Gluten free option
