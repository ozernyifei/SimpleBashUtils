#include "s21_grep.h"

static struct option const long_flags[] = {{NULL, 0, NULL, 0}};

int main(int argc, char *argv[]) {
  regex_t templates[1024];
  Flags flags = {0};
  int is_error = 0;
  int template_counter = 0;

  char flag;
  while (((flag = getopt_long(argc, argv, "e:ivclnhsf:o", long_flags, NULL)) !=
          -1) &&
         (!is_error)) {
    switch (flag) {
      case 'e':
        regcomp(&templates[template_counter++], optarg,
                flags.i ? REG_ICASE : REG_EXTENDED);
        flags.e = 1;
        break;
      case 'i':
        flags.i = 1;
        break;
      case 'v':
        flags.v = 1;
        break;
      case 'c':
        flags.c = 1;
        break;
      case 'l':
        flags.l = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
      case 'h':
        flags.h = 1;
        break;
      case 's':
        flags.s = 1;
        break;
      case 'f':
        flags.f = 1;
        is_error =
            ReadTemplateFromFile(templates, &template_counter, flags, optarg);
        break;
      case 'o':
        flags.o = 1;
        break;
      case '?':
        is_error = 1;
        WrongUsage(GREP_ERROR);
        break;
      default:
        is_error = 1;
        WrongUsage(GREP_ERROR);
        break;
    }
  }
  if (!is_error && (optind + 1 - (flags.f || flags.e)) < argc) {
    if (!(flags.f || flags.e))
      regcomp(&templates[template_counter++], argv[optind++],
              flags.i ? REG_ICASE : REG_EXTENDED);
    if (optind == argc - 1) {
      flags.h = 1;
    }
    while (optind < argc) {
      if (FlagsProcessing(templates, template_counter, argv[optind], flags) &&
          !flags.s)
        WrongUsage(GREP_ERROR);
      optind++;
    }
  } else
    WrongUsage(GREP_ERROR);

  FreeTemplates(templates, template_counter);

  return 0;
}

int FlagsProcessing(regex_t *templates, int templates_counter, char *filename,
                    Flags flags) {
  int is_error = 0;
  int buffer_length = 0;
  int line_count = 0;
  int match_count = 0;
  int line_position = 0;
  char line[4096];
  char match = 0;
  FILE *fp = fopen(filename, "r");
  is_error = !fp;

  while (!is_error && feof(fp) == 0 && fgets(line, 4096, fp)) {
    match = 0;
    buffer_length = strlen(line);
    line_count++;
    for (int i = 0; i < templates_counter; i++) {
      line_position = 0;
      regmatch_t regmatch;
      while (!regexec(&templates[i], &line[line_position], 1, &regmatch, 0)) {
        if (flags.o && !flags.v && !flags.c && !flags.l) {
          if (!line_position && !flags.h) printf("%s:", filename);
          if (!line_position && flags.n) printf("%d:", line_count);
          while (regmatch.rm_so < regmatch.rm_eo) {
            printf("%c", line[line_position + regmatch.rm_so]);
            regmatch.rm_so++;
          }
          printf("\n");
        }
        match = 1;
        line_position += regmatch.rm_eo;
      }
    }
    if (flags.v) match = !match;

    match_count += match;

    if (match && !flags.c && !flags.l && (!flags.o || flags.v)) {
      if (!flags.h) printf("%s:", filename);
      if (flags.n) printf("%d:", line_count);
      for (int i = 0; i < buffer_length && line[i] != '\n'; i++)
        printf("%c", line[i]);
      printf("\n");
    }
  }

  if (!is_error && flags.c) {
    if (!flags.h) printf("%s:", filename);
    printf("%d\n", flags.l ? !!match_count : match_count);
  }

  if (flags.l && !is_error)
    if (match_count > 0) printf("%s\n", filename);

  if (!is_error) fclose(fp);

  return is_error;
}

void FreeTemplates(regex_t *templates, int count) {
  for (int i = 0; i < count; i++) regfree(&templates[i]);
}

int ReadTemplateFromFile(regex_t *templates, int *templates_counter,
                         Flags flags, char *filename) {
  int is_error = 0;
  int buffer_length = 0;
  char buffer[1024];

  FILE *fp = fopen(filename, "r");
  is_error = !fp;

  while (fgets(buffer, 1024, fp) && !is_error) {
    buffer_length = strlen(buffer);

    if (buffer[buffer_length - 1] == '\n') {
      buffer[buffer_length - 1] = '\0';
    }

    regcomp(&templates[(*templates_counter)++], buffer,
            flags.i ? REG_ICASE : REG_EXTENDED);
  }
  if (!is_error) {
    fclose(fp);
  }
  return is_error;
}