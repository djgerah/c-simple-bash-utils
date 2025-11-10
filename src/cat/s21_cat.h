#ifndef S21_CAT
#define S21_CAT

#include <stdbool.h>
#include <stdio.h>

#define OK 0
#define ERROR 1

typedef struct {
  bool b, e, v, n, s, t;
} cat_opt;

void handle_error();
void print_hint(char *argv[]);
cat_opt initialization();
void process_e(cat_opt *opt);
void process_tv(cat_opt *opt);
void process_b(cat_opt *opt);
void process_cat(cat_opt *opt, FILE *file);
void s21_cat_file(const char *file_name, cat_opt *opt);
int parse_arguments(int argc, char *argv[], cat_opt *opt);
int run_s21_cat(int argc, char *argv[]);

#endif