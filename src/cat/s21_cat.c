#include "s21_cat.h"

#include <getopt.h>

void handle_error() {
  fprintf(stderr, "Usage: s21_cat [-beEnstT] [file ...]\n");
  fprintf(stderr, "Try 's21_cat --help' for more information.\n");
}

void print_hint(char *argv[]) {
  fprintf(stderr, "s21_cat: unknown option %s\n", argv[optind - 1]);
  fprintf(stderr, "Try 's21_cat --help' for more information.\n");
}

cat_opt initialization() {
  cat_opt opt;

  opt.b = false;
  opt.e = false;
  opt.n = false;
  opt.s = false;
  opt.t = false;
  opt.v = false;

  return opt;
}

/*
 * Отображение символов конца строки как $
 * -e предполагает и -v
 */
void process_e(cat_opt *opt) {
  opt->e = true;
  opt->v = true;
}

/*
 *	Отображение табов как ^I
 * -t предполагает и -v
 */
void process_tv(cat_opt *opt) {
  opt->t = true;
  opt->v = true;
}

// Нумерация только непустых строк
void process_b(cat_opt *opt) {
  if (opt->b) {
    opt->n = false;
  }
}

static void print_line_number(int *line_num, bool *new_line) {
  printf("%6d\t", (*line_num)++);
  *new_line = false;
}

static void process_char(cat_opt *opt, bool *new_line, int cur_char,
                         char *prev_char, int *line_num,
                         bool *previous_line_empty) {
  // Нумерация только не пустых строк
  if (opt->b && *new_line && cur_char != '\n') {
    print_line_number(line_num, new_line);
    // Нумерация всех выходных строк
  } else if (opt->n && *new_line) {
    print_line_number(line_num, new_line);
  }

  // Сжатие пустых строк (-s)
  if (opt->s && cur_char == '\n' && *prev_char == '\n') {
    if (*previous_line_empty) return;
    *previous_line_empty = true;
  } else {
    *previous_line_empty = false;
  }

  // Отображение конца строки (-e)
  if (opt->e && cur_char == '\n') {
    putchar('$');
  }

  // Отображение табуляции (-t)
  if (opt->t && cur_char == '\t') {
    putchar('^');
    cur_char = 'I';
  }

  // Отображение специальных символов (-v)
  if (opt->v && cur_char != '\n' && cur_char != '\t') {
    if (cur_char >= 0 && cur_char <= 31) {
      putchar('^');
      cur_char += 64;
    } else if (cur_char == 127) {
      putchar('^');
      cur_char -= 64;
    }
  }

  // Печать символа
  putchar(cur_char);

  *new_line = (cur_char == '\n');
  *prev_char = cur_char;
}

void process_cat(cat_opt *opt, FILE *file) {
  int cur_char;
  bool new_line = true;
  bool previous_line_empty = false;
  int line_num = 1;
  char prev_char = 0;

  while ((cur_char = fgetc(file)) != EOF) {
    process_char(opt, &new_line, cur_char, &prev_char, &line_num,
                 &previous_line_empty);
  }
}

void s21_cat_file(const char *file_name, cat_opt *opt) {
  FILE *file = fopen(file_name, "r");

  if (file == NULL) {
    fprintf(stderr, "s21_cat: %s: No such file or directory\n", file_name);
    return;
  }

  process_cat(opt, file);
  fclose(file);
}

void handle_long_option(struct option ln_opts[], int id, cat_opt *opt) {
  if (*ln_opts[id].flag == 0) {
    // -b (GNU: --number-nonblank)
    opt->b = true;
  } else if (*ln_opts[id].flag == 1) {
    // -n (GNU: --number)
    opt->n = true;
  } else if (*ln_opts[id].flag == 2) {
    // -s (GNU: --squeeze-blank)
    opt->s = true;
  }
}

bool fill_cat_opt(int argc, char *argv[], cat_opt *opt,
                  struct option ln_opts[]) {
  int option, id = 0;
  int exit_code = OK;

  while ((option = getopt_long(argc, argv, "beEnstTv", ln_opts, &id)) != -1) {
    switch (option) {
      case 0:
        handle_long_option(ln_opts, id, opt);
        break;
      case 'b':
        opt->b = true;
        break;
      case 'e':
        process_e(opt);
        break;
      case 'E':
        // GNU only: -E то же самое, что и -е, но без применения -v
        opt->e = true;
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
        // (GNU: -T то же самое, но без применения -v)
        opt->t = true;
        break;
      case 'v':
        opt->v = true;
        break;
      default:
        print_hint(argv);
        exit_code = ERROR;
        break;
    }
  }

  return exit_code;
}

int parse_arguments(int argc, char *argv[], cat_opt *opt) {
  struct option ln_opts[] = {{"number-nonblank", no_argument, NULL, 'b'},
                             {"number", no_argument, NULL, 'n'},
                             {"squeeze-blank", no_argument, NULL, 's'},
                             {NULL, 0, NULL, 0}};

  int exit_code = fill_cat_opt(argc, argv, opt, ln_opts);

  process_b(opt);

  return exit_code;
}

void process_files(cat_opt *opt, int argc, char *argv[]) {
  for (int i = optind; i < argc; i++) {
    s21_cat_file(argv[i], opt);
  }
}

int run_s21_cat(int argc, char *argv[]) {
  int exit_code = OK;
  cat_opt opt = initialization();
  exit_code = parse_arguments(argc, argv, &opt);

  if (exit_code == OK) {
    process_files(&opt, argc, argv);
  } else {
    exit_code = ERROR;
  }

  return exit_code;
}

int main(int argc, char *argv[]) {
  int exit_code = OK;

  if (argc < 2) {
    fprintf(stderr, "s21_cat: too few arguments.\n");
    handle_error();
    exit_code = ERROR;
  }

  if (exit_code == OK) {
    exit_code = run_s21_cat(argc, argv);
  }

  return exit_code;
}