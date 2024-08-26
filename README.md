# A custom HTTP server that implements an N-Puzzle Solver
Author: [Jack Robbins](https://www.github.com/jackr276)

## Introduction
This project implements a fully custom HTTP server that interfaces with a new and improved version of the [N-Puzzle Solver](https://github.com/jackr276/N-Puzzle-Solver) to allow for a web based interface. This project aims to solve the performance and stability shortcomings of many other N-Puzzle Solvers that exist online and have been written in Javascript to run client side. Running the N-Puzzle solver client side and in Javascript makes the entire website prone to crashes and freezes, due to the poor performance of the language. Everything about this project has been written from scratch, from the request handling to the N-Puzzle Solver, in an attempt to become more knowledgeable about computer networking and as a way to make it as performant as possible by running everything server-side. The HTTP server itself is multithreaded, and as such can scale to dozens or even hundreds of active clients at once, given the appropriate server hardware. This project was definitely a great learning experience that touched several domains of programming, and this write-up contains many of the important details.

## Design Philosophy
This project implements a multithreaded HTTP server that allows for several different web clients to interact with the solver at once. This means that the number of concurrent connections and solvers running is practically only limited by the resources of the host computer. Below is a diagram of how the server works, from startup to shutdown, and handling a request:    

![Untitled Diagram drawio(1)](https://github.com/user-attachments/assets/90e05cde-bcb8-4110-aab4-8a9a22e89c06)


## Modules/Subsystems
This project was designed with future usability and expansion in mind. As such, I designed several independent subsystems that each server their own purpose. The subsystems are:
### [HTTP Request Parser](https://github.com/jackr276/N-Puzzle-Solver-HTTP-Server/tree/main/src/server/http_parser)
This subsystem contains a full parser that is designed to parse HTTP requests, pick out the relevant data and request type, and return these data as a conveniently packaged struct for the server to use. The functions in this subsystem are invoked whenever a new request from a client is received, as the request type dictates the response type.

### [N-Puzzle Solver](https://github.com/jackr276/N-Puzzle-Solver-HTTP-Server/tree/main/src/server/npuzzle)
For the purposes of this project, this subsystem does all of the work when it comes to generating and solving N-Puzzle problems using an A* search algorithm with several heuristics. The entire design and strategy of the [N-Puzzle Solver Subsystem](https://github.com/jackr276/N-Puzzle-Solver) is discussed in the linked repository. I will not show it here in the interest of brevity, but anyone who is interested is able to view it there. 

### [Response Builder](https://github.com/jackr276/N-Puzzle-Solver-HTTP-Server/tree/main/src/server/response_builder)
The response builder subsystem is responsible for constructing the HTML that will be served in response to a request. The HTML constructed is based on the type of the request, which is why the parser subsystem is needed. Unlike some web servers, the HTML here is not served up through files, but as strings that are constructed and then sent over the socket connection. This decision was made because it is impossible to predict how long a solution to a certain N-Puzzle is, and as such impossible to make a set HTML file to serve up. This way of doing things allows for the server to dynamically serve data based on the solution to the given problem.

### [Server](https://github.com/jackr276/N-Puzzle-Solver-HTTP-Server/tree/main/src/server/remote_server)
The server subsystem is the one that glues the previous 3 together. It uses sockets to maintain connections and send data, and is designed to be able to handle several concurrent connections at once. This is achieved through multithreading, and each client connection being granted its own individual server thread. The source code files themselves are very well documented and easy to read, so if you're interested in further in depth details of the subsystem, I'd recommend looking through them yourself.

## Running this program
After cloning the repository and navigating to the directory where it was downloaded, run the following command to give the runner script executable privileges:
```console
example@bash: ~/N-Puzzle-Solver-HTTP-Server $ chmod +x run.sh
```
Following this step, the video below details the full functionality of the project, and how to use it:   

https://github.com/user-attachments/assets/658c3008-3850-417f-8e93-d1c65d70f9fc

