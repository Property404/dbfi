# Dagan's Brainfuck Interpreter
DBFI is a Brainfuck interpreter based off my [Hyper-Dimensional Brainfuck](https://github.com/Property404/hdbf) interpreter(hdbf). This is a much faster implementation of Brainfuck.

## Command line options:
`-d`	Allow debugging commands (`~`)  
`-h` or `--help`	Print help message  
`-i cmd`	Pass argument as code instead of filename  
`-o`	Optimize before running  
`-t`	Display run time of program  
`-v` or `--version`	Display DBFI version number

## Installing
    git clone https://github.com/Property404/dbfi
    cd dbfi
    make;sudo make install
    # test
    bf examples/LostKingdom.b

## Possible future features:  
Forbidding wrapping  
Gluten free option
