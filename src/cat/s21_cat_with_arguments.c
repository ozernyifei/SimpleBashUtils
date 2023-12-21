#include "s21_cat.h"

static struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"squeze-blank", no_argument, NULL, 's'},
    {"number", no_argument, NULL, 'n'},
    {"show-nonprinting", no_argument, NULL, 'v'},
    {NULL, 0, NULL, 0}};

int ReadFlags(int argc, char **argv, Flags *flags) {
  int flag = 0;
  int is_error = 0;
  while ((flag = getopt_long(argc, argv, "benstvET", long_options, NULL)) !=
         -1) {
    switch (flag) {
      case 'b':
        flags->number_nonblank = 1;
        flags->count++;
        break;
      case 'e':
        flags->show_endline = 1;
        flags->show_nonprinting = 1;
        flags->count++;
        break;
      case 'n':
        flags->number_all = 1;
        // flags->number_nonblank == 0 ? 1 : 0;
        flags->count++;
        break;
      case 's':
        flags->squeeze_blank = 1;
        flags->count++;
        break;
      case 't':
        flags->show_tabs = 1;
        flags->show_nonprinting = 1;
        flags->count++;
        break;
      case 'v':
        flags->show_nonprinting = 1;
        flags->count++;
        break;
      case 'E':
        flags->show_endline = 1;
        flags->count++;
        break;
      case 'T':
        flags->show_tabs = 1;
        flags->count++;
        break;
      case '?':
        WrongUsage(CAT_ERROR);
        is_error = 1;
        break;
      default:
        is_error = 1;
        break;
    }
  }
  return is_error;
}

/*Required flags:
-b (--number-nonblank) done
-e (GNU: -E (no -v)) not done
-n (--number) done
-s (--squeeze-blank) not done
-t (GNU: -T(no -v)) done?
-v fix
*/

void CatWithFlagsNoDecomposition(Flags flags, char *filename) {
  int empty_line_count = 0;
  int line_count = 1;
  char *buffer = NULL;
  size_t len;
  ssize_t char_count;
  FILE *fp = fopen(filename, "r");

  while ((char_count = getline(&buffer, &len, fp)) != -1) {
    if (flags.squeeze_blank && empty_line_count && (buffer[0] == '\n')) {
      empty_line_count++;
    } else {
      empty_line_count = (buffer[0] == '\n') ? 1 : 0;
      if (flags.number_nonblank) {
        if (buffer[0] != '\n') {
          PrintNumber(line_count);
          line_count++;
        }
      } else if (flags.number_all) {
        PrintNumber(line_count);
        line_count++;
      }
      if (flags.show_nonprinting) {
        PrintNonprint(buffer, char_count, flags);
      } else {
        PrintEndlineOrTabsOrNothing(buffer, flags, char_count);
      }
    }
  }
  if (buffer) {
    free(buffer);
  }
  if (fp) {
    fclose(fp);
  }
}
void PrintEndlineOrTabsOrNothing(char *buffer, Flags flags, int char_count) {
  for (int i = 0; i < char_count; i++) {
    if (flags.show_tabs && buffer[i] == '\t') {
      printf("^I");
    } else {
      printf("%c", buffer[i]);
    }
    if (flags.show_endline && buffer[char_count - 1] == i &&
        buffer[char_count - 1] == '\n') {
      printf("$");
    }
  }
}
void PrintNonprint(char *buffer, int char_count,
                   Flags flags) {  // v printNonprint char as m-notation
  for (int i = 0; i < char_count; i++) {
    char symbol = buffer[i];
    if (i == char_count - 1 && symbol == '\n') {
      flags.show_endline ? printf("$%c", symbol) : printf("%c", symbol);
    } else {
      if (symbol < 0) {
        symbol += 256;
      }
      if (symbol == '\t') {
        flags.show_tabs ? printf("^I") : printf("\t");
      }
      // else if (symbol == '\n' && buffer[char_count - 1] == i) {
      //   flags.show_endline ? printf("$") : printf("\n");
      // }
      else if (symbol >= 32) {
        symbol < 127    ? printf("%c", symbol)
        : symbol == 127 ? printf("^?")
                        : printf("M-%c", symbol - 128);
      } else {
        printf("^%c", symbol + 64);
      }
    }
  }
}

void PrintNumber(int num) { printf("%6d\t", num); }
