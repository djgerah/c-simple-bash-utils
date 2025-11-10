#include "s21_grep.h"

#include <regex.h>
#include <string.h>

void handle_error() {
  fprintf(stderr, "Usage: [OPTION]... PATTERN [FILE]...\n");
  fprintf(stderr, "Try 's21_grep --help' for more information.\n");
}

grep_opt initialization() {
  grep_opt opt;

  opt.patterns = NULL;
  opt.pattern_count = 0;
  opt.e = false;
  opt.n = false;
  opt.h = false;
  opt.v = false;
  opt.c = false;
  opt.l = false;
  opt.s = false;
  opt.i = false;
  opt.f = false;
  opt.o = false;

  return opt;
}

void add_pattern(grep_opt *opt, const char *pattern) {
  char **temp =
      realloc(opt->patterns, (opt->pattern_count + 1) * sizeof(char *));

  if (!temp) {
    fprintf(stderr, "s21_grep: adding pattern failed.\n");
    free_patterns(opt);
    return;
  }

  opt->patterns = temp;
  opt->patterns[opt->pattern_count] = malloc(strlen(pattern) + 1);

  if (!opt->patterns[opt->pattern_count]) {
    fprintf(stderr, "s21_grep: adding pattern failed.\n");
    free_patterns(opt);
  } else {
    strcpy(opt->patterns[opt->pattern_count], pattern);
    opt->pattern_count++;
  }
}

void free_patterns(grep_opt *opt) {
  for (int i = 0; i < opt->pattern_count; i++) {
    free(opt->patterns[i]);
  }

  free(opt->patterns);
}

void print_line(const char *line) {
  size_t len = strlen(line);
  if (len > 0 && line[len - 1] == '\n') {
    printf("%s", line);
  } else {
    printf("%s\n", line);
  }
}

// Добавление шаблона
void process_e(int option, grep_opt *opt) {
  if (optarg) {
    add_pattern(opt, optarg);
  } else {
    fprintf(stderr, "s21_grep: option requires an argument -- '%c'\n", option);

    return;
  }
}

void print_file_name_if_needed(grep_opt *opt, int file_count,
                               const char *file_name) {
  if (!opt->h && file_count > 1) {
    printf("%s:", file_name);
  }
}

// Инвертирование смысла поиска соответствий
void process_v(grep_opt *opt, bool match_found, int file_count,
               const char *file_name, const char *line, int line_number) {
  if (opt->v && !match_found) {
    if (opt->o) return;

    // !opt->h ? Выводить совпадающие строки, предваряя их именами файлов
    print_file_name_if_needed(opt, file_count, file_name);

    if (opt->n) {
      printf("%d:", line_number);
    }

    print_line(line);
  }
}

// Вывод только количества совпадающих строк
void process_c(grep_opt *opt, int file_count, const char *file_name,
               int matches) {
  if (opt->c) {
    if (opt->l) return;

    // !opt->h ? Выводить совпадающие строки, предваряя их именами файлов
    print_file_name_if_needed(opt, file_count, file_name);

    printf("%d\n", matches);
  }
}

// Вывод только совпадающих файлов
void process_l(grep_opt *opt, int matches, const char *file_name) {
  if (opt->l && matches > 0) {
    printf("%s\n", file_name);
  }
}

// Предварение каждой строки вывода номером строки из файла ввода
void process_n(grep_opt *opt, int file_count, int line_number,
               const char *file_name) {
  // !opt->h ? Выводить совпадающие строки, предваряя их именами файлов
  print_file_name_if_needed(opt, file_count, file_name);

  if (opt->n) {
    printf("%d:", line_number);
  }
}

// Получение регулярных выражений из файла
void process_f(char *argv[], int option, grep_opt *opt) {
  const char *file_name = optarg ? optarg : argv[optind - 1];

  if (optarg) {
    FILE *file = fopen(optarg, "r");

    if (file == NULL) {
      if (!opt->s) {
        fprintf(stderr, "s21_grep: %s: No such file or directory\n", file_name);
      }

      return;
    }

    char pattern[SIZE];

    while (fgets(pattern, sizeof(pattern), file) != NULL) {
      pattern[strcspn(pattern, "\n")] = '\0';
      add_pattern(opt, pattern);
    }

    fclose(file);
  } else {
    fprintf(stderr, "s21_grep: option requires an argument -- '%c'\n", option);
  }
}

// Печать всех совпадений шаблона (-o)
void process_reg(grep_opt *opt, int file_count, int line_number,
                 const char *file_name, const char *line) {
  for (int i = 0; i < opt->pattern_count; i++) {
    regex_t preg;
    // opt->i ? Игнорирование различия регистра : Нет
    int cflags = opt->i ? REG_ICASE : 0;

    if (regcomp(&preg, opt->patterns[i], cflags) == 0) {
      regmatch_t pmatch[1];

      while (regexec(&preg, line, 1, pmatch, 0) == 0) {
        process_n(opt, file_count, line_number, file_name);

        printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
               line + pmatch[0].rm_so);

        line += pmatch[0].rm_eo;
      }

      regfree(&preg);
    }
  }
}

// Обработка строки, в которой найдено совпадение
void process_match_found(grep_opt *opt, int file_count, int line_number,
                         const char *file_name, const char *line) {
  // Печать только непустых частей совпавшей строки
  if (opt->o) {
    process_reg(opt, file_count, line_number, file_name, line);
  } else {
    process_n(opt, file_count, line_number, file_name);
    print_line(line);
  }
}

// Обработка строки без совпадений (для -v)
void process_match_not_found(grep_opt *opt, int file_count, int line_number,
                             const char *file_name, const char *line) {
  process_v(opt, /*match_found=*/false, file_count, file_name, line,
            line_number);
}

// Выборка строки для вывода в зависимости от -v и совпадения
void proccess_matching(grep_opt *opt, bool line_match, int file_count,
                       int line_number, const char *file_name,
                       const char *line) {
  if (!opt->v && line_match) {
    process_match_found(opt, file_count, line_number, file_name, line);
  } else if (opt->v && !line_match) {
    process_match_not_found(opt, file_count, line_number, file_name, line);
  }
}

// Проверка строки на соответствие одному шаблону
bool process_pattern(const char *pattern, const char *line, grep_opt *opt) {
  bool match_found = false;
  regex_t preg;
  // opt->i ? Игнорирование различия регистра : Нет
  int cflags = opt->i ? REG_ICASE : 0;

  if (regcomp(&preg, pattern, cflags) == 0) {
    if (regexec(&preg, line, 0, NULL, 0) == 0) {
      match_found = true;
    }

    regfree(&preg);
  }

  return match_found;
}

int process_grep(grep_opt *opt, const int argc, FILE *file,
                 const char *file_name) {
  char line[SIZE];
  int line_number = 0;
  int file_count = argc - optind;
  int matches = 0;

  while (fgets(line, sizeof(line), file) != NULL) {
    ++line_number;
    bool line_match = false;

    for (int i = 0; i < opt->pattern_count; i++) {
      const char *pattern = opt->patterns[i];

      if (process_pattern(pattern, line, opt)) {
        line_match = true;
        if (!opt->o) break;
      }
    }

    bool selected = opt->v ? !line_match : line_match;

    if (selected) {
      matches++;
    }

    if (!opt->c && !opt->l) {
      proccess_matching(opt, line_match, file_count, line_number, file_name,
                        line);
    }
  }

  process_c(opt, file_count, file_name, matches);
  process_l(opt, matches, file_name);

  return matches;
}

int s21_grep_file(grep_opt *opt, const int argc, const char *file_name) {
  FILE *file = fopen(file_name, "r");
  int matches = 0;

  if (file == NULL) {
    if (!opt->s) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", file_name);
    }
  } else {
    matches = process_grep(opt, argc, file, file_name);
    fclose(file);
  }

  return matches;
}

int fill_grep_opt(int argc, char *argv[], grep_opt *opt) {
  int option;
  int exit_code = OK;

  while ((option = getopt(argc, argv, "e:f:ivclnsho")) != -1) {
    switch (option) {
      case 'i':
        opt->i = true;
        break;
      case 'v':
        opt->v = true;
        break;
      case 'c':
        opt->c = true;
        break;
      case 'l':
        opt->l = true;
        break;
      case 'n':
        opt->n = true;
        break;
      case 'h':
        // Вывод совпадающих строк, не предваряя их именами файлов
        opt->h = true;
        break;
      case 's':
        // Подавление сообщений об ошибках о несуществующих или нечитаемых
        // файлах
        opt->s = true;
        break;
      case 'o':
        // Печать только тех участков строки (подстрока), которые соответствуют
        // шаблону (регулярному выражению)
        opt->o = true;
        break;
      case 'e':
        process_e(option, opt);
        break;
      case 'f':
        process_f(argv, option, opt);
        break;
      default:
        fprintf(stderr, "s21_grep: unknown option -%c\n", option);
        exit_code = ERROR;
        break;
    }
  }

  return exit_code;
}

int parse_arguments(int argc, char *argv[], grep_opt *opt) {
  int exit_code = fill_grep_opt(argc, argv, opt);

  if (opt->pattern_count == 0 && optind < argc) {
    add_pattern(opt, argv[optind]);
    ++optind;
  }

  return exit_code;
}

int process_files(grep_opt *opt, int argc, char *argv[]) {
  int exit_code = 1;

  for (int i = optind; i < argc; i++) {
    int matched = s21_grep_file(opt, argc, argv[i]);
    if (matched > 0) {
      exit_code = 0;
    }
  }

  return exit_code;
}

int run_s21_grep(int argc, char *argv[]) {
  grep_opt opt = initialization();
  int exit_code = parse_arguments(argc, argv, &opt);

  if (exit_code == OK) {
    exit_code = process_files(&opt, argc, argv);
  }

  free_patterns(&opt);
  return exit_code;
}

int main(int argc, char *argv[]) {
  int exit_code = OK;

  if (argc < 3) {
    fprintf(stderr, "s21_grep: too few arguments.\n");
    handle_error();
    exit_code = ERROR;
  }

  if (exit_code == OK) {
    exit_code = run_s21_grep(argc, argv);
  }

  return exit_code;
}