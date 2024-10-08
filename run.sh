# Author: Jack Robbins
# A simple runner file that starts up the server

#!/bin/bash

if [[ ! -d ./out ]]; then
	mkdir out
fi

rm -r out/*

read -p "Do you want a remote server run (r) or debug mode(d)? " OPTION

#Compilation commands here
gcc -o ./out/run -Wall -Wextra -pthread ./src/server_run.c \
						   ./src/server/npuzzle/puzzle/puzzle.c \
						   ./src/server/npuzzle/solver/solve_multi_threaded.c \
						   ./src/server/remote_server/server.c \
						   ./src/server/response_builder/response_builder.c \
						   ./src/server/http_parser/parser.c 


./out/run -$OPTION
