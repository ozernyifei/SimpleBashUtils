#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_
#define CAT_ERROR "usage: s21_cat [-benstvET] [file ...]\n"

#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/common.h"

typedef struct flags {
  int number_nonblank;
  int show_endline;
  int squeeze_blank;
  int number_all;
  int show_nonprinting;
  int show_tabs;
  int count;
} Flags;

int ReadFlags(int argc, char **argv, Flags *flags);
void PrintNumber(int num);
void PrintFileNoFlags(const char *filename);
void CatWithFlagsNoDecomposition(Flags flags, char *filename);
void PrintNonprint(char *buffer, int char_count, Flags flags);
void PrintEndlineOrTabsOrNothing(char *buffer, Flags flags, int char_count);
int IsNull(const char *filename);

#endif