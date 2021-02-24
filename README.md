This is a small app for handle requests that get from socket.

##Installation

- download project

##Config

The `config.cnf` file is for set config app such as `request_per_min` and `socket_port`.

##Compile&Run

` cmake --build . --target clean -- -j 2`

`cmake --build . --target all -- -j 2`

`./throttler`