# cserver-demo

A rudimentary stream server-client demo in C.

## Functionality

### Server

1. Sets up a stream socket and binds to 8080, listening for client connection.
1. Upon recv'ing a connection, it accepts and prints the client's message.
1. Sends client an ack, then closes.

### Client

1. Connects to the server on port 8080.
1. Sends a message.
1. Waits for an ack from the server, then closes.

## Demo

Open two terminal windows. In the first one, enter:

```
make server
```

Then, in the second one:

```
make client
```
