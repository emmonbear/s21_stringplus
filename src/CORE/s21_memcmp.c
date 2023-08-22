#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  bool condition = false;
  char *s_str1 = (char *)str1;
  char *s_str2 = (char *)str2;
  int result = 0;
  for (s21_size_t i = 0; i < n; i++, s_str1++, s_str2++) {
    if (!condition) result = *s_str1 - *s_str2;
    if (result != 0) condition = true;
  }
  return result;
}