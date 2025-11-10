#ifndef S21_GREP
#define S21_GREP

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 4096
#define OK 0
#define ERROR 1

typedef struct {
  char **patterns;
  int pattern_count;
  bool e, i, v, c, l, n, h, s, f, o;
} grep_opt;

void handle_error();
grep_opt initialization();
void add_pattern(grep_opt *opt, const char *pattern);
void free_patterns(grep_opt *opt);
void print_line(const char *line);
void process_e(int option, grep_opt *opt);
void process_v(grep_opt *opt, bool match_found, int file_count,
               const char *file_name, const char *line, int line_number);
void process_c(grep_opt *opt, int file_count, const char *file_name,
               int matches);
void process_l(grep_opt *opt, int matches, const char *file_name);
void process_n(grep_opt *opt, int file_count, int line_number,
               const char *file_name);
void process_f(char *argv[], int option, grep_opt *opt);
void process_reg(grep_opt *opt, int file_count, int line_number,
                 const char *file_name, const char *line);
void proccess_matching(grep_opt *opt, bool line_match, int file_count,
                       int line_number, const char *file_name,
                       const char *line);
int process_grep(grep_opt *opt, const int argc, FILE *file,
                 const char *file_name);
int s21_grep_file(grep_opt *opt, const int argc, const char *file_name);
int parse_arguments(int argc, char *argv[], grep_opt *opt);
int run_s21_grep(int argc, char *argv[]);

#endif
