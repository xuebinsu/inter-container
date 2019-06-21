#pragma once

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define PTR_MINUS(x, y) (ptrdiff_t)((size_t)(x) - (size_t)(y))

struct plc_buffer_elem {
  size_t size;
  unsigned char payload[0];
};

struct plc_buffer {
  unsigned char *head;  // head of the buffer
  unsigned char *tail;  // tail of the buffer
  unsigned char *end;   // elements in the buffer ends here
  size_t capacity;      // number of bytes of the data

  unsigned char begin[0];  // elements in the buffer begins here
};

struct plc_buffer *plc_buffer_init(void *addr, size_t size);

struct plc_buffer_elem *plc_buffer_front(struct plc_buffer *buf);
struct plc_buffer_elem *plc_buffer_back(struct plc_buffer *buf);
bool plc_buffer_is_empty(struct plc_buffer *buf);

struct plc_buffer_elem *plc_buffer_push(struct plc_buffer *buf, void *payload,
                                        size_t size);
struct plc_buffer_elem *plc_buffer_pop(struct plc_buffer *buf);

void plc_buffer_compact(struct plc_buffer *buf);
struct plc_buffer_elem *plc_buffer_alloc(struct plc_buffer *buf, size_t size);