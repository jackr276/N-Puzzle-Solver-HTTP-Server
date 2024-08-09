# Author: Jack Robbins
# A simple runner file that starts up the server

#!/bin/bash

if [[ ! -d ./out ]]; then
	mkdir out
fi

rm -r out/*

#Compilation commands here
gcc -Wall -Wextra -pthread ./src/server/server_run.c ./src/server/npuzzle/puzzle/puzzle.c ./src/server/npuzzle/solver/solve_multi_threaded.c ./src/server/remote-server/server.c -o ./out/run


./out/run
