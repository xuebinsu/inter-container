CC = gcc
CCFLAGS = -std=c11 -Wall -Wextra -g
BUILD = build

.PHONY: all
all:
	mkdir -p $(BUILD)
	client server buffer

.PHONY: client
client: 
	mkdir -p $(BUILD)
	$(CC) $(CCFLAGS) -c client.c -o $(BUILD)/client

.PHONY: server
server:
	mkdir -p $(BUILD)
	$(CC) $(CCFLAGS) -c server.c -o $(BUILD)/server

.PHONY: buffer
buffer: 
	mkdir -p $(BUILD)
	$(CC) $(CCFLAGS) -c buffer.c test_buffer.c -o $(BUILD)/buffer

.PHONY: clean
clean:
	rm -rf build/*