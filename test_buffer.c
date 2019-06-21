#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

int x = 0xcafebeef;

void test_push_pop() {
  size_t size = 40 + sizeof(struct plc_buffer);
  void *addr = malloc(size);
  struct plc_buffer *buf = plc_buffer_init(addr, size);
  printf("buffer starts at %p\n", buf->begin);

  struct plc_buffer_elem *elem;
  elem = plc_buffer_push(buf, (unsigned char *)&x, sizeof(x));
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem != NULL);
  elem = plc_buffer_push(buf, (unsigned char *)&x, sizeof(x));
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem != NULL);
  elem = plc_buffer_push(buf, (unsigned char *)&x, sizeof(x));
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem != NULL);

  elem = plc_buffer_push(buf, (unsigned char *)&x, sizeof(x));
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem == NULL);

  elem = plc_buffer_pop(buf);
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem != NULL);
  elem = plc_buffer_push(buf, (unsigned char *)&x, sizeof(x));
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem == NULL);

  elem = plc_buffer_pop(buf);
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem != NULL);
  elem = plc_buffer_push(buf, (unsigned char *)&x, sizeof(x));
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem != NULL);

  elem = plc_buffer_pop(buf);
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem != NULL);
  elem = plc_buffer_push(buf, (unsigned char *)&x, sizeof(x));
  printf("elem at %p, buffer head = %p, buffer tail = %p\n", elem, buf->head,
         buf->tail);
  assert(elem != NULL);
  free(addr);
}

int main() {
  printf("buffer tests starts:\n");
  test_push_pop();
  printf("buffer tests passed!\n");
  return 0;
}
