#include "buffer.h"

struct plc_buffer *plc_buffer_init(void *addr, size_t size) {
  struct plc_buffer *buf = (struct plc_buffer *)addr;
  buf->capacity = size - sizeof(struct plc_buffer);
  buf->head = buf->tail = buf->end = buf->begin;
  return buf;
}

bool plc_buffer_is_empty(struct plc_buffer *buf) {
  return buf->head == buf->tail;
}

size_t plc_buffer_waste_size(struct plc_buffer *buf) {
  return (size_t)PTR_MINUS(buf->begin + buf->capacity, buf->end);
}

size_t plc_buffer_size(struct plc_buffer *buf) {
  ptrdiff_t r = PTR_MINUS(buf->tail, buf->head);
  return r >= 0 ? (size_t)r
                : (size_t)r + buf->capacity - plc_buffer_waste_size(buf);
}

void plc_buffer_compact(struct plc_buffer *buf) {
  if (buf->head != buf->begin) {
    if (plc_buffer_size(buf) > 0) {
      if (buf->tail >= buf->head) {
        size_t size = PTR_MINUS(buf->tail, buf->head);
        memcpy(buf->begin, buf->head, size);
        buf->end = buf->tail = buf->begin + size;
      } else {
        size_t end_size = PTR_MINUS(buf->end, buf->head);
        size_t begin_size = PTR_MINUS(buf->tail, buf->begin);
        memcpy(buf->tail, buf->begin, begin_size);
        memcpy(buf->begin, buf->head, end_size);
        buf->tail += begin_size;
        buf->end = buf->tail;
      }
    }
    buf->head = buf->begin;
  }
}

struct plc_buffer_elem *plc_buffer_alloc(struct plc_buffer *buf, size_t size) {
  size += sizeof(struct plc_buffer_elem);
  if (plc_buffer_size(buf) <= size &&
      // If the buffer is almost full, it is not worth being compacted.
      plc_buffer_size(buf) <= buf->capacity / 2) {
    plc_buffer_compact(buf);
  }
  struct plc_buffer_elem *elem = NULL;
  if (buf->tail >= buf->head) {
    if (PTR_MINUS(buf->begin + buf->capacity, buf->tail) >= (ptrdiff_t)size) {
      elem = (struct plc_buffer_elem *)buf->tail;
      buf->tail += size;
      buf->end = buf->tail;
      return elem;
    } else if (PTR_MINUS(buf->head, buf->begin) - 1 >= (ptrdiff_t)size) {
      buf->end = buf->tail;
      elem = (struct plc_buffer_elem *)(buf->begin);
      buf->tail = buf->begin + size;
      return elem;
    } else {
      return NULL;
    }
  } else {
    if (PTR_MINUS(buf->head, buf->tail) - 1 >= (ptrdiff_t)size) {
      elem = (struct plc_buffer_elem *)buf->tail;
      buf->tail += size;
      return elem;
    } else {
      return NULL;
    }
  }
}

struct plc_buffer_elem *plc_buffer_push(struct plc_buffer *buf, void *payload,
                                        size_t size) {
  struct plc_buffer_elem *elem = plc_buffer_alloc(buf, size);
  if (elem != NULL) {
    elem->size = size;
    memcpy(elem->payload, payload, size);
  }
  return elem;
}

struct plc_buffer_elem *plc_buffer_pop(struct plc_buffer *buf) {
  if (!plc_buffer_is_empty(buf)) {
    struct plc_buffer_elem *elem = (struct plc_buffer_elem *)buf->head;
    buf->head += sizeof(struct plc_buffer_elem) + elem->size;
    if (buf->head >= buf->end) {
      buf->head = buf->begin;
      buf->end = buf->tail;
    }
    return elem;
  }
  return NULL;
}

struct plc_buffer_elem *plc_buffer_front(struct plc_buffer *buf) {
  return (struct plc_buffer_elem *)buf->head;
}
