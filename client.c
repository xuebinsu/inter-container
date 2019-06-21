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

int futex_wake(void *futex_word, int num_procs) {
  return syscall(SYS_futex, futex_word, FUTEX_WAKE, num_procs, 0, 0, 0);
}

#define SHARED_MEM_SIZE 4096

int main() {
  int fd = open("/dev/shm/plc/req", O_CREAT | O_RDWR);
  if (fd == -1) {
    perror("Failed to open shared memory.");
    exit(EXIT_FAILURE);
  }
  ftruncate(fd, SHARED_MEM_SIZE);
  char *req =
      (char *)mmap(NULL, SHARED_MEM_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
  if (req == NULL) {
    perror("Failed to map shared memory.");
    exit(EXIT_FAILURE);
  }
  int *futex_word = (int *)req;
  char *data = req + sizeof(int);
  strcpy(data, "hello, world!\n");
  *futex_word = 1;
  futex_wake(futex_word, 1);
  return 0;
}
