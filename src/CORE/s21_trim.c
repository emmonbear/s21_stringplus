#include "../s21_string.h"

int isTrim(char c, const char *trim_chars) {
  bool res = false;
  if (trim_chars[0] == '\0') {
    res = (c == ' ' || c == '\t' || c == '\n');
  }
  for (s21_size_t i = 0; trim_chars[i] != '\0'; i++) {
    if (c == trim_chars[i]) {
      res = true;
      break;
    }
  }
  return res;
}

void *s21_trim(const char *src, const char *trim_chars) {
  char *result = s21_NULL;
  if (src) {
    if (trim_chars) {
      s21_size_t len = s21_strlen(src);
      const char *start = src;
      const char *end = src + len - 1;
      while (*start != '\0' && isTrim(*start, trim_chars)) {
        start++;
      }
      while (end > start && isTrim(*end, trim_chars)) {
        end--;
      }
      s21_size_t new_len = end - start + 1;
      if (new_len == 0) {
        return s21_NULL;
      }
      result = calloc(new_len + 1, sizeof(char));
      for (s21_size_t i = 0; i < new_len; i++) {
        result[i] = start[i];
      }
      result[new_len] = '\0';
    } else {
      result = s21_trim(src, " \t\n");
    }
  }
  return result;
}