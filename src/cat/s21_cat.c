#include "s21_cat.h"

int main(int argc, char **argv) {
  Flags flags = {0};
  int is_error = 0;
  if (argc == 1) {
    WrongUsage(CAT_ERROR);
  } else {
    is_error = ReadFlags(argc, argv, &flags);
    if (is_error) {
      WrongUsage(CAT_ERROR);
    } else {
      if (optind == argc) {
        WrongUsage(CAT_ERROR);
      }
      for (int i = optind; i < argc; i++) {
        int is_null = IsNull(argv[i]);

        if (flags.count == 0 && !is_null) {
          PrintFileNoFlags(argv[i]);
        } else if (flags.count > 0 && !is_null) {
          CatWithFlagsNoDecomposition(flags, argv[i]);
        } else {
          WrongUsage(CAT_ERROR);
        }
      }
    }
  }
  return 0;
}

void PrintFileNoFlags(const char *filename) {
  char *buffer = NULL;
  size_t len;
  ssize_t char_count;
  FILE *fp = fopen(filename, "r");

  while ((char_count = getline(&buffer, &len, fp)) != -1) {
    printf("%s", buffer);
  }
  if (buffer) free(buffer);
  fclose(fp);
}

int IsNull(const char *filename) {
  FILE *fp = fopen(filename, "r");

  int is_null = NULL == fp ? 1 : 0;

  fclose(fp);

  return is_null;
}