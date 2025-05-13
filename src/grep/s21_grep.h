#ifndef S21_GREP
#define S21_GREP

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 4096

typedef struct {
  char **patterns;
  int pattern_count;
  bool e, i, v, c, l, n, h, s, f, o;
} grep_opt;

void initialization(grep_opt *opt) {
  opt->patterns = NULL;
  opt->pattern_count = 0;
  opt->e = opt->n = opt->h = opt->v = opt->c = opt->l = opt->s = opt->i =
      opt->f = opt->o = false;
}

// bool handle_error();
// void free_patterns(grep_opt *opt);
// bool parse_arguments(int argc, char *argv[], grep_opt *opt);
// void process_f(char option, grep_opt *opt);
// void process_e(char option, grep_opt *opt);
// bool add_pattern(grep_opt *opt, const char *pattern);
// void grep_in_file(grep_opt *opt, const char *file_name, int file_count);
// void process_c(grep_opt *opt, int file_count, const char *file_name,
//                int matches);
// void process_v(grep_opt *opt, bool match_found, int file_count,
//                const char *file_name, const char *line);
// bool process_pattern(const char *pattern, const char *line, grep_opt *opt,
//                      const char *file_name, int line_number,
//                      // int *matches,
//                      int file_count);
// void check_v(grep_opt *opt);
// void process_l(grep_opt *opt, int matches, const char *file_name);

#endif