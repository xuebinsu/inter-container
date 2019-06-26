#include <assert.h>
#include <immintrin.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N (1 << 30)

#define ALIGN 256

void memcpy2(void* dest, const void* src, size_t size) {
  size_t len = size / (ALIGN / 8);
  for (size_t i = 0; i < len; i++) {
    __m256i tmp = _mm256_stream_load_si256(((__m256i*)src) + i);
    _mm256_store_si256(((__m256i*)dest) + i, tmp);
  }
  _mm_mfence();
}

void memcpy3(void* dest, const void* src, size_t size) {
  for (size_t i = 0; i < size / sizeof(int); i++) {
    int tmp = ((int*)src)[i];
    ((int*)dest)[i] = tmp;
  }
}

int main() {
  void* src = aligned_alloc(ALIGN, N);
  memset(src, 1, N);
  printf("memset src = %d\n", ((char*)src)[0]);
  void* dest = aligned_alloc(ALIGN, N);
  memset(dest, 0, N);
  for (int i = 0; i < N; i++) {
    assert(((char*)dest)[i] == 0);
  }

  clock_t begin = clock();
  memcpy2(dest, src, N);
  clock_t end = clock();
  double time_spent = (double)(end - begin);
  for (int i = 0; i < N; i++) {
    assert(((char*)dest)[i] == 1);
  }
  printf("memcpy2 src = %d, dest = %d, time = %f\n", ((char*)src)[N - 1],
         ((char*)dest)[N - 1], time_spent);

  begin = clock();
  memcpy(dest, src, N);
  end = clock();
  time_spent = (double)(end - begin);
  printf("memcpy src = %d, dest = %d, time = %f\n", ((char*)src)[N - 1],
         ((char*)dest)[N - 1], time_spent);

  begin = clock();
  memcpy3(dest, src, N);
  end = clock();
  time_spent = (double)(end - begin);
  printf("memcpy3 src = %d, dest = %d, time = %f\n", ((char*)src)[N - 1],
         ((char*)dest)[N - 1], time_spent);
  return 0;
}