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

#endif