#include "s21_grep.h"

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool handle_error() {
  fprintf(stderr, "Usage: [OPTION]... PATTERN [FILE]...\n");
  fprintf(stderr, "Try 's21_grep --help' for more information.\n");
  return true;
}

void free_patterns(grep_opt *opt) {
  for (int i = 0; i < opt->pattern_count; i++) {
    free(opt->patterns[i]);
  }
  free(opt->patterns);
}

bool add_pattern(grep_opt *opt, const char *pattern) {
  if (!pattern) return false;

  char **new_patterns =
      realloc(opt->patterns, (opt->pattern_count + 1) * sizeof(char *));
  if (!new_patterns) {
    fprintf(stderr, "Memory allocation failed\n");
    return false;
  }

  opt->patterns = new_patterns;
  opt->patterns[opt->pattern_count] = malloc(strlen(pattern) + 1);
  if (!opt->patterns[opt->pattern_count]) {
    fprintf(stderr, "Memory allocation failed\n");
    return false;
  }

  strcpy(opt->patterns[opt->pattern_count], pattern);
  opt->pattern_count++;

  return true;
}

void process_c(grep_opt *opt, int file_count, const char *file_name,
               int matches) {
  if (opt->c) {
    if (file_count > 1) {
      printf("%s:", file_name);
    }
    printf("%d\n", matches);
  }
}

void process_l(grep_opt *opt, int matches, const char *file_name) {
  if (opt->l && matches > 0) {
    printf("%s\n", file_name);
  }
}

void process_e(char option, grep_opt *opt) {
  if (optarg) {
    add_pattern(opt, optarg);
  } else {
    fprintf(stderr, "s21_grep: option requires an argument -- '%c'\n", option);
    exit(1);
  }
}

void process_f(char option, grep_opt *opt) {
  if (optarg) {
    FILE *file = fopen(optarg, "r");
    if (!file) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", optarg);
      exit(1);
    }

    char pattern[SIZE];
    while (fgets(pattern, sizeof(pattern), file)) {
      pattern[strcspn(pattern, "\n")] = '\0';
      add_pattern(opt, pattern);
    }
    fclose(file);
  } else {
    fprintf(stderr, "s21_grep: option requires an argument -- '%c'\n", option);
    exit(1);
  }
}

bool parse_arguments(int argc, char *argv[], grep_opt *opt) {
  bool is_error = false;
  int option;

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
        opt->h = true;
        break;
      case 's':
        opt->s = true;
        break;
      case 'o':
        opt->o = true;
        break;
      case 'e':
        process_e(option, opt);
        break;
      case 'f':
        process_f(option, opt);
        break;
      default:
        fprintf(stderr, "s21_grep: unknown option -%c\n", option);
        is_error = handle_error();
    }
  }

  if (opt->pattern_count == 0 && optind < argc) {
    add_pattern(opt, argv[optind++]);
  }

  return is_error;
}

void process_reg(grep_opt *opt, int file_count, int line_number,
                 const char *file_name, const char *line, const char *pattern) {
  if (!opt->c && !opt->l && !opt->v) {
    if (!opt->h && file_count > 1) {
      printf("%s:", file_name);
    }
    if (opt->n) {
      printf("%d:", line_number);
    }

    if (opt->o) {
      regex_t reg;
      int flags = REG_EXTENDED | (opt->i ? REG_ICASE : 0);
      if (regcomp(&reg, pattern, flags) == 0) {
        regmatch_t pmatch[1];
        const char *search_ptr = line;

        while (regexec(&reg, search_ptr, 1, pmatch, 0) == 0) {
          printf("%.*s\n", (int)(pmatch[0].rm_eo - pmatch[0].rm_so),
                 search_ptr + pmatch[0].rm_so);
          search_ptr += pmatch[0].rm_eo;
        }
        regfree(&reg);
      }
    } else {
      printf("%s", line);
    }
  }
}

bool process_pattern(const char *pattern, const char *line, grep_opt *opt,
                     const char *file_name, int line_number, int *matches,
                     int file_count) {
  regex_t reg;
  int flags = REG_EXTENDED | (opt->i ? REG_ICASE : 0);
  bool match_found = false;

  if (regcomp(&reg, pattern, flags) == 0) {
    if (regexec(&reg, line, 0, NULL, 0) == 0) {
      (*matches)++;
      match_found = true;
      process_reg(opt, file_count, line_number, file_name, line, pattern);
    }
    regfree(&reg);
  }

  return match_found;
}

void grep_in_file(grep_opt *opt, const char *file_name) {
  FILE *file = fopen(file_name, "r");
  if (!file) {
    if (!opt->s) {
      fprintf(stderr, "s21_grep: %s: No such file or directory\n", file_name);
    }
    return;
  }

  char line[SIZE];
  int line_number = 0, matches = 0;
  int file_count = opt->pattern_count;

  while (fgets(line, sizeof(line), file) != NULL) {
    bool match_found = false;
    ++line_number;

    for (int i = 0; i < opt->pattern_count; i++) {
      const char *pattern = opt->patterns[i];
      match_found = process_pattern(pattern, line, opt, file_name, line_number,
                                    &matches, file_count);
    }

    if (opt->v && !match_found) {
      matches++;
    }
  }

  fclose(file);
  if (opt->c) printf("%d\n", matches);
  if (opt->l && matches > 0) printf("%s\n", file_name);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "s21_grep: too few arguments.\n");
    return 1;
  }

  grep_opt opt;
  initialization(&opt);

  if (parse_arguments(argc, argv, &opt)) {
    return 1;
  }

  for (int i = optind; i < argc; i++) {
    grep_in_file(&opt, argv[i]);
  }

  free_patterns(&opt);
  
  return 0;
}