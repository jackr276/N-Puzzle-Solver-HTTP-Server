# A custom HTTP server that implements an N-Puzzle Solver
Author: [Jack Robbins](https://www.github.com/jackr276)

## Introduction
This project implements a fully custom HTTP server that interfaces with a new and improved version of the [N-Puzzle Solver](https://github.com/jackr276/N-Puzzle-Solver) to allow for a web based interface. This project aims to solve the performance and stability shortcomings of many other N-Puzzle Solvers that exist online and have been written in Javascript to run client side. Running the N-Puzzle solver client side and in Javascript makes the entire website prone to crashes and freezes, due to the poor performance of the language. Everything about this project has been written from scratch, from the request handling to the N-Puzzle Solver, in an attempt to become more knowledgeable about computer networking and as a way to make it as performant as possible by running everything server-side. The HTTP server itself is multithreaded, and as such can scale to dozens or even hundreds of active clients at once, given the appropriate server hardware. This project was definitely a great learning experience that touched several domains of programming, and this write-up contains many of the important details.

## Running this program
After cloning the repository and navigating to the directory where it was downloaded, run the following command to give the runner script executable privileges:
```console
example@bash: ~/N-Puzzle-Solver-HTTP-Server $ chmod +x run.sh
```
Following this step, the video below details the full functionality of the project, and how to use it:   

https://github.com/user-attachments/assets/658c3008-3850-417f-8e93-d1c65d70f9fc

