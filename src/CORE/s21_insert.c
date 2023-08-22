#include "../s21_string.h"

void* s21_insert(const char* src, const char* str, s21_size_t start_index) {
  char* result = s21_NULL;

  if (src != s21_NULL && str != s21_NULL) {
    s21_size_t len_src = s21_strlen(src);
    s21_size_t len_str = s21_strlen(str);
    s21_size_t new_len = len_src + len_str;

    if (start_index > len_src) {
      return s21_NULL;
    }

    result = calloc(new_len + 1, sizeof(char));

    if (result) {
      s21_size_t i;  // для прохода по исходной строке

      // вставляем символы из src в result до места вставки
      for (i = 0; i < start_index; i++) {
        result[i] = src[i];
      }

      // вставляем в result строку, которую нужно вставить
      for (s21_size_t j = 0; j < len_str; j++) {
        result[i++] = str[j];
      }

      // вставляем остатки
      for (s21_size_t j = start_index; j < len_src; j++) {
        result[i++] = src[j];
      }

      result[new_len] = '\0';
    }
  }

  if (src != s21_NULL && str == s21_NULL) {
    result = calloc(s21_strlen(src) + 1, sizeof(char));

    if (result) {
      for (s21_size_t i = 0; i < s21_strlen(src); i++) {
        result[i] = src[i];
      }
      result[s21_strlen(src)] = '\0';
    }
  }
  return (void*)result;
}