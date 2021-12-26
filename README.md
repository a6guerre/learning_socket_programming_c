# learning_socket_programming_c

The purpose of this repository is to learn socket programming in C by implementing common client-server models, multithreaded clients, and slightly more advanced socket progamming techniques (In progress).

For the multithreaded server example, the exercise was derived from a post I came about: https://eli.thegreenplace.net/2017/concurrent-servers-part-1-introduction/ and https://eli.thegreenplace.net/2017/concurrent-servers-part-2-threads/
Please read part one to acquint yourself with the protocol. The server implements a protocol in which the client can trigger state transitions in  a state machine. There are two states, )) the server is waiting for a message. 2) Server is recieiving a message and will construct a response to client based on the message.

To run:
cd multithreaded_server_client/
make

./server

On seperate tab.
./run.sh
