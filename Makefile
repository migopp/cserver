CCOMP		= clang
CFLAGS		= -std=c99 -Wall -Wextra

CLIENT_SRC 	= client.c
SERVER_SRC 	= server.c

CLIENT	= target/client
SERVER	= target/server

all: $(CLIENT) $(SERVER)

client: $(CLIENT)
	./target/client

server: $(SERVER)
	./target/server

$(CLIENT): $(CLIENT_SRC)
	mkdir -p $(dir $@)
	$(CCOMP) $(CFLAGS) -o $@ $^

$(SERVER): $(SERVER_SRC)
	mkdir -p $(dir $@)
	$(CCOMP) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGET_DIR)

rebuild: clean all

.PHONY: all client server clean rebuild
