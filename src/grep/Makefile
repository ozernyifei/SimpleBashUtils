s21_grep:
	gcc -Wall -Wextra -Werror s21_grep.c ../common/common.c -o s21_grep

check:
	clang-format -n -style=Google *.c *.h

fix:
	clang-format -i -style=Google *.c *.h

clean:
	rm -rf *.o s21_grep 
