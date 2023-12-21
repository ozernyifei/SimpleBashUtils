

s21_cat: 
	gcc s21_cat.c s21_cat_with_arguments.c ../common/common.c -o s21_cat

check: 
	clang-format -n -style = Google *.c *.h

fix: 
	clang-format -i -style=Google *.c *.h

clean:
	rm -rf *.o s21_cat
