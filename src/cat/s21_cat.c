#include "s21_cat.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool handle_error() {
  fprintf(stderr, "Usage: s21_cat [-beEnstT] [file ...]\n");
  fprintf(stderr, "Try 's21_cat --help' for more information.\n");
  return true;
}

void process_e(cat_opt *opt) {
  opt->e = true;
  opt->v = true;
}

void process_tv(cat_opt *opt) {
  opt->t = true;
  opt->v = true;
}

void check_b(cat_opt *opt) {
  if (opt->b) {
    opt->n = false;
  }
}

bool parse_arguments(int argc, char *argv[], cat_opt *opt) {
  int option;
  bool is_error = false;
  struct option ln_opts[] = {{"number-nonblank", no_argument, NULL, 'b'},
                             {"number", no_argument, NULL, 'n'},
                             {"squeeze-blank", no_argument, NULL, 's'},
                             {NULL, 0, NULL, 0}};

  while ((option = getopt_long(argc, argv, "beEnstTv", ln_opts, NULL)) != -1) {
    switch (option) {
      case 'b':
        opt->b = true;
        break;
      case 'e':
        process_e(opt);
        break;
      case 'E':
        // opt->e = true;
        process_e(opt);
        break;
      case 'n':
        opt->n = true;
        break;
      case 's':
        opt->s = true;
        break;
      case 't':
        process_tv(opt);
        break;
      case 'T':
        opt->t = true;
        break;
      case 'v':
        opt->v = true;
        break;
      default:
        fprintf(stderr, "s21_cat: unknown option %s\n", argv[optind - 1]);
        is_error = handle_error();
        break;
    }
  }

  check_b(opt);  // Ensure -b overrides -n
  return is_error;
}

void cat_file(const char *file_name, cat_opt *opt) {
  FILE *file = fopen(file_name, "r");
  if (file == NULL) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", file_name);
    return;
  }

  int cur_char;
  bool new_line = true;
  bool last_was_blank = false;
  int line = 1;

  while ((cur_char = fgetc(file)) != EOF) {
    // Handle -s (squeeze blank lines)
    if (opt->s && cur_char == '\n') {
      if (last_was_blank) {
        continue;
      }
      last_was_blank = true;
    } else {
      last_was_blank = false;
    }

    // Handle -b (number non-blank lines) and -n (number all lines)
    if (new_line) {
      if (opt->b && cur_char != '\n') {
        printf("%6d\t", line++);
      } else if (opt->n && !opt->b) {
        printf("%6d\t", line++);
      }
      new_line = false;
    }

    // Handle -E (show $ at the end of each line)
    if (opt->e && cur_char == '\n') {
      putchar('$');
    }

    // Handle -T (show ^I for tabs)
    if (opt->t && cur_char == '\t') {
      putchar('^');
      cur_char = 'I';
    }

    // Handle -v (show non-printable characters)
    if (opt->v && (cur_char != '\n' && cur_char != '\t')) {
      if (cur_char >= 0 && cur_char <= 31) {
        putchar('^');
        cur_char += 64;
      } else if (cur_char == 127) {
        putchar('^');
        cur_char -= 64;
      }
    }

    new_line = (cur_char == '\n');

    putchar(cur_char);
  }

  fclose(file);
}

bool cat_process(int argc, char *argv[], cat_opt *opt) {
  bool is_error = false;

  if (parse_arguments(argc, argv, opt) == 0) {
    for (int i = optind; i < argc; i++) {
      cat_file(argv[i], opt);
    }
  } else {
    is_error = true;
  }

  return is_error;
}

void initialization(cat_opt *opt) {
  opt->b = false;
  opt->e = false;
  opt->n = false;
  opt->s = false;
  opt->t = false;
  opt->v = false;
}

int main(int argc, char *argv[]) {
  bool is_error = false;

  if (argc < 2) {
    fprintf(stderr, "s21_cat: too few arguments.\n");
    is_error = handle_error();
  } else {
    cat_opt opt;
    initialization(&opt);

    is_error = cat_process(argc, argv, &opt);
  }

  return is_error ? 1 : 0;
}