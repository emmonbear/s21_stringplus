#include <limits.h>

#include "../s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int counter = 0;
  for (int i = 0; format[i] != '\0'; i += 1) {
    if (format[i] == '%') {
      options configuration = {"XXXXX", 0,     false,   false,
                               false,   false, INT_MIN, 'X'};
      i = find_configuration(i, format, &configuration, &args);
      counter +=
          process_specifiers(str + counter, format[i], configuration, &args);
    } else {
      str[counter++] = format[i];
      str[counter] = '\0';
    }
  }
  va_end(args);
  return counter;
}

int find_configuration(int i, const char *format, options *configuration,
                       va_list *args) {
  for (i += 1; s21_strchr("-+ #0", format[i]); i += 1)
    (configuration->flags)[5 - s21_strlen(s21_strchr("-+ #0", format[i]))] =
        'o';
  for (; s21_strchr("0123456789", format[i]); i += 1)
    configuration->width = (configuration->width * 10) + (format[i] - 48);
  for (; format[i] == '*'; i += 1) configuration->width = va_arg(*args, int);
  if (format[i] == '.') {
    configuration->flag_zero_trigger = true;
    for (i += 1; format[i] == '*'; i += 1)
      configuration->accuracy = va_arg(*args, int);
    for (; s21_strchr("0123456789", format[i]); i += 1)
      configuration->accuracy =
          (configuration->accuracy * 10) + (format[i] - 48);
  }
  for (; s21_strchr("lhL", format[i]); i += 1)
    configuration->length = format[i];
  return i;
}
int process_specifiers(char *str, char symbol, options configuration,
                       va_list *args) {
  int accuracy = set_accurancy(configuration.accuracy, symbol);
  if (symbol == '%')
    *str = symbol;
  else if (s21_strchr("di", symbol))
    process_specifier_d(str, args, configuration, accuracy);
  else if (s21_strchr("c", symbol))
    process_specifier_c(str, args, configuration);
  else if (s21_strchr("f", symbol)) {
    int length = 0;
    if (configuration.length == 'L')
      process_specifier_f_long(str, va_arg(*args, long double), configuration,
                               accuracy, length);
    else
      process_specifier_f(str, va_arg(*args, double), configuration, accuracy,
                          length);
  } else if (s21_strchr("s", symbol))
    process_specifier_s(str, args, configuration, accuracy);
  else if (s21_strchr("u", symbol))
    process_specifier_u(str, args, configuration, accuracy);
  else if (s21_strchr("e", symbol)) {
    char symbol_tmp = 'e';
    if (configuration.length == 'L')
      process_specifier_e_long(str, va_arg(*args, long double), configuration,
                               accuracy, symbol_tmp);
    else
      process_specifier_e(str, va_arg(*args, double), configuration, accuracy,
                          symbol_tmp);
  } else if (s21_strchr("E", symbol)) {
    char symbol_tmp = 'E';
    if (configuration.length == 'L')
      process_specifier_e_long(str, va_arg(*args, long double), configuration,
                               accuracy, symbol_tmp);
    else
      process_specifier_e(str, va_arg(*args, double), configuration, accuracy,
                          symbol_tmp);
  } else if (s21_strchr("g", symbol)) {
    char symbol_tmp = 'e';
    if (configuration.length == 'L')
      process_specifier_g_long(str, va_arg(*args, long double), configuration,
                               accuracy, symbol_tmp);
    else
      process_specifier_g(str, va_arg(*args, double), configuration, accuracy,
                          symbol_tmp);
  } else if (s21_strchr("G", symbol)) {
    char symbol_tmp = 'E';
    if (configuration.length == 'L')
      process_specifier_g_long(str, va_arg(*args, long double), configuration,
                               accuracy, symbol_tmp);
    else
      process_specifier_g(str, va_arg(*args, double), configuration, accuracy,
                          symbol_tmp);
  } else if (s21_strchr("x", symbol)) {
    char symbol_tmp = 'a';
    char symbol_tmp_1 = 'x';
    process_specifier_x(str, args, configuration, accuracy, symbol_tmp,
                        symbol_tmp_1);
  } else if (s21_strchr("X", symbol)) {
    char symbol_tmp = 'A';
    char symbol_tmp_1 = 'X';
    process_specifier_x(str, args, configuration, accuracy, symbol_tmp,
                        symbol_tmp_1);
  } else if (s21_strchr("o", symbol))
    process_specifier_o(str, args, configuration, accuracy);
  else if (s21_strchr("p", symbol))
    process_specifier_p(str, args, configuration);
  return s21_strlen(str);
}

char *process_specifier_d(char *str, va_list *args, options configuration,
                          int accuracy) {
  int length = 0;
  int minus = 0;
  if (configuration.length == 'l') {
    long int number = va_arg(*args, long int);
    minus = (number < 0) ? (number *= (-1)) : 0;
    integer_part_process_long(str, number, &length, configuration, accuracy);
  } else if (configuration.length == 'h') {
    short int number = va_arg(*args, int);
    minus = (number < 0) ? (number *= (-1)) : 0;
    integer_part_process_short(str, number, &length, configuration, accuracy);
  } else {
    int number = va_arg(*args, int);
    minus = (number < 0) ? (number *= (-1)) : 0;
    integer_part_process(str, number, &length, configuration, accuracy);
  }
  if ((configuration.flags[4] == 'o') && (!configuration.flag_zero_trigger)) {
    flag_zero(str, configuration, length, minus);
  } else {
    minus != 0 ? str[length++] = '-' : 0;
    if ((str[length - 1] != '-') &&
        ((configuration.flags[1] == 'o') || (configuration.flags[2] == 'o')))
      str[length++] = configuration.flags[1] == 'o' ? '+' : ' ';
    width_process(str, length, configuration);
  }
  return str;
}

char *width_process(char *str, int length, options configuration) {
  int begin = length;
  if (configuration.flags[0] == 'o') {
    reverse(str);
    while (begin < configuration.width) str[begin++] = ' ';
  } else {
    while (begin < configuration.width) str[begin++] = ' ';
    reverse(str);
  }

  // str[length] = '\0';
  return str;
}

char *reverse(char *str) {
  int tmp = s21_strlen(str);
  for (int i = 0; i < (tmp / 2); i += 1) {
    char c = str[tmp - 1 - i];
    str[tmp - 1 - i] = str[i];
    str[i] = c;
  }
  return str;
}

int process_specifier_c(char *str, va_list *args, options configuration) {
  int counter = 0;
  for (int i = counter;
       configuration.flags[0] != 'o' && i < configuration.width - 1; i++)
    str[counter++] = ' ';
  str[counter++] = va_arg(*args, int);
  if (configuration.flags[0] == 'o')
    for (int i = counter; i < configuration.width; i++) str[counter++] = ' ';
  str[counter] = '\0';
  return counter;
}

char *process_specifier_f(char *str, double number, options configuration,
                          int accuracy, int length) {
  int minus = 0;
  configuration.trigger_f = true;
  if (number < 0) {
    number = -number;
    minus = 1;
  }
  int integer = (accuracy == 0) ? round(number) : number;
  if (accuracy != 0) {
    double double_tmp =
        ceil((number - trunc(number)) * pow(10, accuracy) - 0.5);
    if (double_tmp >= pow(10, accuracy)) {
      integer += 1;
      double_tmp = 0;
    }
    int decimal = double_tmp;
    for (; ((length < accuracy)) || ((decimal / 10) != 0) ||
           ((decimal % 10) != 0);
         decimal /= 10)
      str[length++] = (decimal % 10) + 48;
    configuration.trigger_f = false;
    str[length++] = '.';
  }
  if (integer == 0)
    str[length++] = '0';
  else
    integer_part_process(str, integer, &length, configuration, accuracy);
  if (configuration.flags[4] == 'o')
    flag_zero(str, configuration, length, minus);
  else {
    minus != 0 ? str[length++] = '-' : 0;
    if ((str[length - 1] != '-') &&
        ((configuration.flags[1] == 'o') || (configuration.flags[2] == 'o')))
      str[length++] = configuration.flags[1] == 'o' ? '+' : ' ';
    width_process(str, length, configuration);
  }
  return str;
}

char *process_specifier_f_long(char *str, long double number,
                               options configuration, int accuracy,
                               int length) {
  int minus = 0;
  configuration.trigger_f = true;
  if (number < 0) {
    number = -number;
    minus = 1;
  }
  long int integer = (accuracy == 0) ? roundl(number) : number;
  if (accuracy != 0) {
    long double double_tmp =
        ceill((number - truncl(number)) * powl(10, accuracy) - 0.5);
    if (double_tmp >= pow(10, accuracy)) {
      integer += 1;
      double_tmp = 0;
    }
    long int decimal = double_tmp;
    for (; ((length < accuracy)) || ((decimal / 10) != 0) ||
           ((decimal % 10) != 0);
         decimal /= 10)
      str[length++] = (decimal % 10) + 48;
    configuration.trigger_f = false;
    str[length++] = '.';
  }
  integer_part_process_long(str, integer, &length, configuration, accuracy);
  if (configuration.flags[4] == 'o')
    flag_zero(str, configuration, length, minus);
  else {
    minus != 0 ? str[length++] = '-' : 0;
    if ((str[length - 1] != '-') &&
        ((configuration.flags[1] == 'o') || (configuration.flags[2] == 'o')))
      str[length++] = configuration.flags[1] == 'o' ? '+' : ' ';
    width_process(str, length, configuration);
  }
  return str;
}

char *integer_part_process(char *str, int number, int *length,
                           options configuration, int accuracy) {
  if (number == 0 && !configuration.trigger_f) {
    for (int i = 0; i < accuracy; i++) str[(*length)++] = '0';
  } else {
    for (;
         ((*length) < accuracy) || ((number / 10) != 0) || ((number % 10) != 0);
         number /= 10)
      str[(*length)++] = (number % 10) + 48;
  }
  return str;
}

char *uinteger_part_process(char *str, unsigned int number, int *length,
                            options configuration, int accuracy) {
  if (number == 0 && !configuration.trigger_f) {
    str[(*length)++] = '0';
  } else {
    for (;
         ((*length) < accuracy) || ((number / 10) != 0) || ((number % 10) != 0);
         number /= 10)
      str[(*length)++] = (number % 10) + 48;
  }
  return str;
}

char *process_specifier_s(char *str, va_list *args, options configuration,
                          int accuracy) {
  int i = 0;
  char *str_tmp = va_arg(*args, char *);
  int length = (accuracy == INT_MIN) ? (int)s21_strlen(str_tmp) : accuracy;
  for (; i < length; i++) str[i] = str_tmp[length - i - 1];
  width_process(str, i, configuration);
  return str;
}

char *process_specifier_u(char *str, va_list *args, options configuration,
                          int accuracy) {
  int length = 0;
  int minus = 0;
  if (configuration.length == 'l') {
    unsigned long int number = va_arg(*args, unsigned long int);
    uinteger_part_process_long(str, number, &length, configuration, accuracy);
  } else if (configuration.length == 'h') {
    unsigned short int number = va_arg(*args, unsigned int);
    uinteger_part_process_short(str, number, &length, configuration, accuracy);
  } else {
    unsigned int number = va_arg(*args, unsigned int);
    uinteger_part_process(str, number, &length, configuration, accuracy);
  }
  if ((configuration.flags[4] == 'o') && (!configuration.flag_zero_trigger)) {
    flag_zero(str, configuration, length, minus);
  } else {
    if (configuration.flags[1] == 'o' || configuration.flags[2] == 'o')
      str[length++] = configuration.flags[1] == 'o' ? '+' : ' ';
    width_process(str, length, configuration);
  }
  return str;
}

int set_accurancy(int accuracy, int symbol) {
  if (accuracy < 0) {
    s21_strchr("diuxXo", symbol) ? accuracy = 1 : 0;
    s21_strchr("feEgG", symbol) ? accuracy = 6 : 0;
  }
  return accuracy;
}

char *integer_part_process_long(char *str, long int number, int *length,
                                options configuration, int accuracy) {
  if (number == 0 && !configuration.trigger_f) {
    for (int i = 0; i < accuracy; i++) str[(*length)++] = '0';
  } else {
    for (;
         ((*length) < accuracy) || ((number / 10) != 0) || ((number % 10) != 0);
         number /= 10)
      str[(*length)++] = (number % 10) + 48;
  }
  return str;
}

char *uinteger_part_process_long(char *str, unsigned long int number,
                                 int *length, options configuration,
                                 int accuracy) {
  if (number == 0 && !configuration.trigger_f) {
    str[(*length)++] = '0';
  } else {
    for (;
         ((*length) < accuracy) || ((number / 10) != 0) || ((number % 10) != 0);
         number /= 10)
      str[(*length)++] = (number % 10) + 48;
  }
  return str;
}

char *integer_part_process_short(char *str, short int number, int *length,
                                 options configuration, int accuracy) {
  if (number == 0 && !configuration.trigger_f) {
    for (int i = 0; i < accuracy; i++) str[(*length)++] = '0';
  } else {
    for (;
         ((*length) < accuracy) || ((number / 10) != 0) || ((number % 10) != 0);
         number /= 10)
      str[(*length)++] = (number % 10) + 48;
  }
  return str;
}

char *uinteger_part_process_short(char *str, short unsigned int number,
                                  int *length, options configuration,
                                  int accuracy) {
  if (number == 0 && !configuration.trigger_f) {
    str[(*length)++] = '0';
  } else {
    for (;
         ((*length) < accuracy) || ((number / 10) != 0) || ((number % 10) != 0);
         number /= 10)
      str[(*length)++] = (number % 10) + 48;
  }
  return str;
}

char *flag_zero(char *str, options configuration, int length, int minus) {
  configuration.width = (configuration.flags[3] == 'o')
                            ? configuration.width - 2
                            : configuration.width;
  configuration.width =
      (minus != 0) ? configuration.width - 1 : configuration.width;
  while (length < configuration.width) str[length++] = '0';
  minus != 0 ? str[length++] = '-' : 0;
  if ((str[length - 1] != '-') &&
      ((configuration.flags[1] == 'o') || (configuration.flags[2] == 'o')))
    str[length++] = configuration.flags[1] == 'o' ? '+' : ' ';
  reverse(str);
  return str;
}

char *process_specifier_e(char *str, double number, options configuration,
                          int accuracy, char symbol) {
  configuration.trigger_e = true;
  int exponent = 0;
  int length = 0;
  bool zero_before_exponent = false;
  bool negative_exponent = false;
  bool exponent_eq_zero = false;
  if (number != 0) {
    exponent = (int)floor(log10(fabs(number)));
    number /= pow(10, exponent);
  }
  zero_before_exponent = (exponent < 10) ? true : false;
  if (exponent < 0) {
    exponent = -exponent;
    negative_exponent = true;
  }
  if (exponent == 0) exponent_eq_zero = true;
  for (; ((exponent / 10) != 0) || ((exponent % 10) != 0); exponent /= 10)
    str[length++] = (exponent % 10) + 48;
  if (exponent_eq_zero) str[length++] = '0';
  if (zero_before_exponent) str[length++] = '0';
  if ((exponent >= 0) && (!negative_exponent))
    str[length++] = '+';
  else
    str[length++] = '-';
  str[length++] = symbol;
  process_specifier_f(str, number, configuration, accuracy, length);
  configuration.trigger_e = false;
  return str;
}

char *process_specifier_e_long(char *str, long double number,
                               options configuration, int accuracy,
                               char symbol) {
  configuration.trigger_e = true;
  int exponent = 0;
  int length = 0;
  bool zero_before_exponent = false;
  bool negative_exponent = false;
  bool exponent_eq_zero = false;

  if (number != 0) {
    exponent = (long int)floorl(log10l(fabsl(number)));
    number /= powl(10.0L, exponent);
  }
  zero_before_exponent = (exponent < 10) ? true : false;
  if (exponent < 0) {
    exponent = -exponent;
    negative_exponent = true;
  }
  if (exponent == 0) exponent_eq_zero = true;
  for (; ((exponent / 10) != 0) || ((exponent % 10) != 0); exponent /= 10)
    str[length++] = (exponent % 10) + 48;
  if (exponent_eq_zero) str[length++] = '0';
  if (zero_before_exponent) str[length++] = '0';
  if ((exponent >= 0) && (!negative_exponent))
    str[length++] = '+';
  else
    str[length++] = '-';
  str[length++] = symbol;
  process_specifier_f_long(str, number, configuration, accuracy, length);
  configuration.trigger_e = false;
  return str;
}

char *process_specifier_g(char *str, double number, options configuration,
                          int accuracy, char symbol) {
  int length = 0;
  int integer = (int)floor(fabs(number));
  int count = (int)log10(integer) + 1;
  if (accuracy >= count) {
    if (accuracy > 6) accuracy = 6;
    accuracy = accuracy - count;
    process_specifier_f(str, number, configuration, accuracy, length);
  } else {
    accuracy = (accuracy >= 6) ? 5 : (accuracy == 0) ? accuracy : accuracy - 1;
    configuration.trigger_g = true;
    process_specifier_e(str, number, configuration, accuracy, symbol);
    configuration.trigger_g = false;
  }
  return str;
}

char *process_specifier_g_long(char *str, long double number,
                               options configuration, int accuracy,
                               char symbol) {
  int length = 0;
  int integer = (int)floorl(fabsl(number));
  int count = (int)log10(integer) + 1;
  if (accuracy >= count) {
    if (accuracy > 6) accuracy = 6;
    accuracy = accuracy - count;
    process_specifier_f_long(str, number, configuration, accuracy, length);
  } else {
    accuracy = (accuracy >= 6) ? 5 : (accuracy == 0) ? accuracy : accuracy - 1;
    configuration.trigger_g = true;
    process_specifier_e_long(str, number, configuration, accuracy, symbol);
    configuration.trigger_g = false;
  }
  return str;
}

char *process_specifier_x(char *str, va_list *args, options configuration,
                          int accuracy, char symbol_tmp, char symbol_tmp_1) {
  int length = 0;
  if (configuration.length == 'h') {
    unsigned short int number = va_arg(*args, unsigned int);
    hexadecimal_conversion_short(str, number, &length, symbol_tmp);
  } else if (configuration.length == 'l') {
    unsigned long int number = va_arg(*args, unsigned long int);
    hexadecimal_conversion_long(str, number, &length, symbol_tmp);
  } else {
    unsigned int number = va_arg(*args, unsigned int);
    hexadecimal_conversion(str, number, &length, symbol_tmp);
  }
  while (length < accuracy) str[length++] = '0';
  if ((configuration.flags[3] == 'o') && (configuration.flags[4] != 'o')) {
    str[length++] = symbol_tmp_1;
    str[length++] = '0';
  }
  if (configuration.flags[4] == 'o') {
    while (length + 2 < configuration.width) str[length++] = '0';
    str[length++] = symbol_tmp_1;
    str[length++] = '0';
    reverse(str);
  } else
    width_process(str, length, configuration);
  return str;
}

char *hexadecimal_conversion(char *str, unsigned int number, int *length,
                             char symbol_tmp) {
  while (number != 0) {
    int digit = number % 16;
    str[(*length)++] = (digit < 10) ? (digit + '0') : (digit - 10 + symbol_tmp);
    number /= 16;
  }
  return str;
}

char *hexadecimal_conversion_short(char *str, unsigned short int number,
                                   int *length, char symbol_tmp) {
  while (number != 0) {
    int digit = number % 16;
    str[(*length)++] = (digit < 10) ? (digit + '0') : (digit - 10 + symbol_tmp);
    number /= 16;
  }
  return str;
}

char *hexadecimal_conversion_long(char *str, unsigned long int number,
                                  int *length, char symbol_tmp) {
  while (number != 0) {
    long int digit = number % 16;
    str[(*length)++] = (digit < 10) ? (digit + '0') : (digit - 10 + symbol_tmp);
    number /= 16;
  }
  return str;
}

char *process_specifier_o(char *str, va_list *args, options configuration,
                          int accuracy) {
  int length = 0;
  if (configuration.length == 'h') {
    unsigned short int number = va_arg(*args, unsigned int);
    octal_conversion_short(str, number, &length);
  } else if (configuration.length == 'l') {
    unsigned long int number = va_arg(*args, unsigned long int);
    octal_conversion_long(str, number, &length);
  } else {
    unsigned int number = va_arg(*args, unsigned int);
    octal_conversion(str, number, &length);
  }
  while (length < accuracy) str[length++] = '0';
  if ((configuration.flags[3] == 'o') && (configuration.flags[4] != 'o'))
    if (str[length - 1] != '0') str[length++] = '0';
  if (configuration.flags[4] == 'o') {
    while (length < configuration.width) str[length++] = '0';
    reverse(str);
  } else
    width_process(str, length, configuration);
  return str;
}

char *octal_conversion(char *str, unsigned int number, int *length) {
  while (number != 0) {
    int digit = number % 8;
    str[(*length)++] = digit + '0';
    number /= 8;
  }
  return str;
}

char *octal_conversion_long(char *str, unsigned long int number, int *length) {
  while (number != 0) {
    long int digit = number % 8;
    str[(*length)++] = digit + '0';
    number /= 8;
  }
  return str;
}

char *octal_conversion_short(char *str, unsigned short int number,
                             int *length) {
  while (number != 0) {
    short int digit = number % 8;
    str[(*length)++] = digit + '0';
    number /= 8;
  }
  return str;
}

char *process_specifier_p(char *str, va_list *args, options configuration) {
  uintptr_t ptr_value = (uintptr_t)va_arg(*args, void *);
  int length = 0;
  while (ptr_value != 0) {
    int digit = ptr_value % 16;
    str[length++] = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
    ptr_value /= 16;
  }
  str[length++] = 'x';
  str[length++] = '0';
  width_process(str, length, configuration);
  return str;
}
