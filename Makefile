CC = gcc
CCFLAGS = -std=c11 -Wall -Wextra -g
BUILD = build

.PHONY: serde
serde:
	mkdir -p $(BUILD)
	$(CC) -std=c11 -Wall -Wextra serde.c -mavx2 -O2 -o $(BUILD)/serde

.PHONY: client
client: 
	mkdir -p $(BUILD)
	$(CC) $(CCFLAGS) client.c -o $(BUILD)/client

.PHONY: server
server:
	mkdir -p $(BUILD)
	$(CC) $(CCFLAGS) server.c -o $(BUILD)/server

.PHONY: buffer
buffer: 
	mkdir -p $(BUILD)
	$(CC) $(CCFLAGS) buffer.c test_buffer.c -o $(BUILD)/buffer

.PHONY: clean
clean:
	rm -rf build/*