#ifndef S21_CAT
#define S21_CAT

#include <getopt.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct {
  bool b, e, v, n, s, t;
} cat_opt;

inline void initialization(cat_opt *opt) {
  opt->b = false;
  opt->e = false;
  opt->n = false;
  opt->s = false;
  opt->t = false;
  opt->v = false;
}

bool handle_error();
void process_e(cat_opt *opt);
void process_tv(cat_opt *opt);
void check_b(cat_opt *opt);
bool parse_arguments(int argc, char *argv[], cat_opt *opt);
void cat_file(const char *file_name, cat_opt *opt);

#endif