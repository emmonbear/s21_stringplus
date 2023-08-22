#include "../s21_string.h"

void *s21_to_upper(const char *str) {
  char *result = s21_NULL;

  if (str != s21_NULL) {
    s21_size_t str_len = s21_strlen(str);
    result = calloc(str_len + 1, sizeof(char));

    for (s21_size_t i = 0; i < str_len; i++) {
      if (str[i] >= 'a' && str[i] <= 'z')
        result[i] = str[i] - 32;
      else
        result[i] = str[i];
    }
    result[str_len] = '\0';
  }
  return result;
}