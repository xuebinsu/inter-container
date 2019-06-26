#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <linux/futex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#define SHARED_MEM_SIZE 4096

int futex_wait(void *futex_word, int value) {
  return syscall(SYS_futex, futex_word, FUTEX_WAKE, value, 0, 0, 0);
}

int main() {
  int fd = -1;
  while (fd == -1) {
    fd = open("/dev/shm/plc/req", O_RDONLY);
  }
  char *req = mmap(NULL, SHARED_MEM_SIZE, PROT_READ, MAP_SHARED, fd, 0);
  if (req == NULL) {
    perror("Failed to map shared memory.");
    exit(EXIT_FAILURE);
  }
  int *futex_word = (int *)req;
  char *data = req + sizeof(int);
  futex_wait(futex_word, 0);
  printf("Message: %s\n", data);
  close(fd);
  return 0;
}