#include <immintrin.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void memcpy2(void* dest, const void* src, size_t size) {
  for (size_t i = 0; i < size / 8; i++) {
    __m256i tmp = _mm256_stream_load_si256(((__m256i*)src));
    _mm256_store_si256(((__m256i*)dest), tmp);
  }
  _mm_mfence();
}

#define N 1 << 30

#define ALIGN 256

int main() {
  void* src = aligned_alloc(ALIGN, N);
  memset(src, 1, N);
  printf("memset src = %d\n", ((char*)src)[0]);
  void* dest = aligned_alloc(ALIGN, N);
  memset(dest, 0, N);

  clock_t begin = clock();
  memcpy2(dest, src, N);
  clock_t end = clock();
  double time_spent = (double)(end - begin);
  printf("memcpy2 src = %d, dest = %d, time = %f\n", ((char*)src)[0],
         ((char*)dest)[0], time_spent);

  begin = clock();
  memcpy(dest, src, N);
  end = clock();
  time_spent = (double)(end - begin);
  printf("memcpy src = %d, dest = %d, time = %f\n", ((char*)src)[0],
         ((char*)dest)[0], time_spent);
  return 0;
}