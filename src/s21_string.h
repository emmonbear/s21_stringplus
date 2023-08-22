#ifndef S21_STRING_H
#define S21_STRING_H
#include <check.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Header/s21_sprintf.h"
#define s21_NULL ((void *)0)

typedef unsigned long s21_size_t;

// Выполняет поиск первого вхождения символа c (беззнаковый тип) в первых n
// байтах строки, на которую указывает аргумент str.
void *s21_memchr(const void *str, int c, size_t n);

// Сравнивает первые n байтов str1 и str2.
int s21_memcmp(const void *str1, const void *str2, size_t n);

// Копирует n символов из src в dest.
void *s21_memcpy(void *dest, const void *src, size_t n);

// Копирует символ c (беззнаковый тип) в первые n символов строки, на которую
// указывает аргумент str.
void *s21_memset(void *str, int c, size_t n);

// Добавляет строку, на которую указывает src, в конец строки, на которую
// указывает dest, длиной до n символов.
char *s21_strncat(char *dest, const char *src, size_t n);

// Выполняет поиск первого вхождения символа c (беззнаковый тип) в строке, на
// которую указывает аргумент str.
char *s21_strchr(const char *str, int c);

// Сравнивает не более первых n байтов str1 и str2.
int s21_strncmp(const char *str1, const char *str2, size_t n);

// Копирует до n символов из строки, на которую указывает src, в dest.
char *s21_strncpy(char *dest, const char *src, size_t n);

// Вычисляет длину начального сегмента str1, который полностью состоит из
// символов, не входящих в str2.
size_t s21_strcspn(const char *str1, const char *str2);

// Вычисляет длину строки str, не включая завершающий нулевой символ.
size_t s21_strlen(const char *str);

// Находит первый символ в строке str1, который соответствует любому символу,
// указанному в str2.
char *s21_strpbrk(const char *str1, const char *str2);

// Выполняет поиск во внутреннем массиве номера ошибки errnum и возвращает
// указатель на строку с сообщением об ошибке. Нужно объявить макросы,
// содержащие массивы сообщений об ошибке для операционных систем mac и linux.
// Описания ошибок есть в оригинальной библиотеке. Проверка текущей ОС
// осуществляется с помощью директив.
char *s21_strerror(int errnum);

// Выполняет поиск последнего вхождения символа c (беззнаковый тип) в строке, на
// которую указывает аргумент str.
char *s21_strrchr(const char *str, int c);

// Находит первое вхождение всей строки needle (не включая завершающий нулевой
// символ), которая появляется в строке haystack.
char *s21_strstr(const char *haystack, const char *needle);

// Разбивает строку str на ряд токенов, разделенных delim.
char *s21_strtok(char *str, const char *delim);

int s21_sprintf(char *str, const char *format, ...);

// Вставляет строку в строку
void *s21_insert(const char *src, const char *str, s21_size_t start_index);

// Замена верхнего регистра на нижний
void *s21_to_lower(const char *str);

// Замена нижнего регистра на верхний
void *s21_to_upper(const char *str);

// Функция для обрезания символов
void *s21_trim(const char *src, const char *trim_chars);

// Проверяет, следует ли обрезать строку
int isTrim(char c, const char *trim_chars);

#endif
