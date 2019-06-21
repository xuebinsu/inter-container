CC = gcc
CCFLAGS = -std=c11 -Wall -Wextra -g
BUILD = build

.PHONY: client
client: 
	$(CC) $(CCFLAGS) -c client.c -o $(BUILD)/client

.PHONY: server
server:
	$(CC) $(CCFLAGS) -c server.c -o $(BUILD)/server

.PHONY: buffer
buffer: 
	$(CC) $(CCFLAGS) -c buffer.c test_buffer.c -o $(BUILD)/buffer

.PHONY: clean
clean:
	rm -rf build/*