#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H
#include <math.h>
#include <stdint.h>

typedef struct {
  char flags[10];
  int width;
  bool trigger_f;
  bool trigger_e;
  bool trigger_g;
  bool flag_zero_trigger;
  int accuracy;
  char length;
} options;

int s21_sprintf(char *str, const char *format, ...);
int find_configuration(int i, const char *format, options *configuration,
                       va_list *args);
int process_specifiers(char *str, char symbol, options configuration,
                       va_list *args);
int process_specifier_c(char *str, va_list *args, options configuration);
char *process_specifier_d(char *str, va_list *args, options configuration,
                          int accuracy);
char *reverse(char *str);
char *process_specifier_f(char *str, double number, options configuration,
                          int accuracy, int length);
char *process_specifier_f_long(char *str, long double number,
                               options configuration, int accuracy, int length);
char *integer_part_process(char *str, int number, int *length,
                           options configuration, int accuracy);
char *uinteger_part_process(char *str, unsigned int number, int *length,
                            options configuration, int accuracy);
char *width_process(char *str, int length, options configuration);
char *process_specifier_s(char *str, va_list *args, options configuration,
                          int accuracy);
char *process_specifier_u(char *str, va_list *args, options configuration,
                          int accuracy);
int set_accurancy(int accuracy, int symbol);
char *integer_part_process_long(char *str, long int number, int *length,
                                options configuration, int accuracy);
char *uinteger_part_process_long(char *str, unsigned long int number,
                                 int *length, options configuration,
                                 int accuracy);
char *integer_part_process_short(char *str, short int number, int *length,
                                 options configuration, int accuracy);
char *uinteger_part_process_short(char *str, short unsigned int number,
                                  int *length, options configuration,
                                  int accuracy);
char *flag_zero(char *str, options configuration, int length, int minus);
char *process_specifier_e(char *str, double number, options configuration,
                          int accuracy, char symbol);
char *process_specifier_e_long(char *str, long double number,
                               options configuration, int accuracy,
                               char symbol);
char *process_specifier_g(char *str, double number, options configuration,
                          int accuracy, char symbol);
char *process_specifier_g_long(char *str, long double number,
                               options configuration, int accuracy,
                               char symbol);
char *process_specifier_x(char *str, va_list *args, options configuration,
                          int accuracy, char symbol_tmp, char symbol_tmp_1);
char *hexadecimal_conversion(char *str, unsigned int number, int *length,
                             char symbol_tmp);
char *hexadecimal_conversion_short(char *str, unsigned short int number,
                                   int *length, char symbol_tmp);
char *hexadecimal_conversion_long(char *str, unsigned long int number,
                                  int *length, char symbol_tmp);
char *process_specifier_o(char *str, va_list *args, options configuration,
                          int accuracy);
char *octal_conversion(char *str, unsigned int number, int *length);
char *octal_conversion_long(char *str, unsigned long int number, int *length);
char *octal_conversion_short(char *str, unsigned short int number, int *length);
char *process_specifier_p(char *str, va_list *args, options configuration);

#endif  // S21_SPRINTF_H
