CC 			= clang
CFLAGS		= -std=c99 -Wall -Wextra

SRC_DIR 	= src
CLIENT_DIR	= $(SRC_DIR)/client
SERVER_DIR	= $(SRC_DIR)/server

CLIENT_SRC 	= $(wildcard $(CLIENT_DIR)/*.c)
SERVER_SRC 	= $(wildcard $(SERVER_DIR)/*.c)

CLIENT	= target/client
SERVER	= target/server

all: $(CLIENT) $(SERVER)

client: $(CLIENT)
	./target/client

server: $(SERVER)
	./target/server

$(CLIENT): $(CLIENT_SRC)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER): $(SERVER_SRC)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf $(TARGET_DIR)

rebuild: clean all

.PHONY: all client server clean rebuild
