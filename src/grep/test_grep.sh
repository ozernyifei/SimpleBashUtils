#!/bin/sh

EXPECTED=/tmp/expected.txt
GOT=/tmp/got.txt
Red='\033[0;31m'
Green='\033[0;32m'
NC='\033[0m'

while IFS= read -r line; do
    grep $line > "$EXPECTED" 2> /dev/null < /dev/null
    ./s21_grep $line > "$GOT" 2> /dev/null < /dev/null
    if cmp -s "$EXPECTED" "$GOT"; then
        echo "TEST s21_grep $line ${Green}PASSED${NC}"
    else
        echo "TEST s21_grep $line ${Red}FAILED${NC}"
        
    fi
done <<END
s21_grep.c
-e str s21_grep.c
-v str s21_grep.c
-i str s21_grep.c
-n str s21_grep.c
-c str s21_grep.c
-l str s21_grep.c
-e str s21_grep.c Makefile
-v str s21_grep.c Makefile
-i str s21_grep.c Makefile
-n str s21_grep.c Makefile
-c str s21_grep.c Makefile
-l str s21_grep.c Makefile
-e else -e Return s21_grep.c
-e else -e Return -n s21_grep.c
-e if -e Return -v s21_grep.c Makefile
-l -h -e as -c s21_grep.c
-v -e lo s21_grep.c
-e ret -l -c s21_grep.c Makefile
-e str -e return  -v -i -n s21_grep.c
-l -i -e ReT -c -n -v -e else s21_grep.c Makefile
not_exists.txt
s21_grep.c not_exists.txt
END