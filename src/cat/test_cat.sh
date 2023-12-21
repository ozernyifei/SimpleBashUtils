#!/bin/sh

EXPECTED=/tmp/expected.txt
GOT=/tmp/got.txt
Red='\033[0;31m'
Green='\033[0;32m'
NC='\033[0m'

while IFS= read -r line; do
    cat $line > "$EXPECTED" 2> /dev/null < /dev/null
    ./s21_cat $line > "$GOT" 2> /dev/null < /dev/null
    if cmp -s "$EXPECTED" "$GOT"; then
        echo "TEST s21_cat $line ${Green}PASSED${NC}"
    else
        echo "TEST s21_cat $line ${Red}FAILED${NC}"
        
    fi
done <<END
s21_cat.c
-b s21_cat.c
-e s21_cat.c
-n s21_cat.c
-s s21_cat.c
-t s21_cat.c
-e s21_cat.c Makefile
-s s21_cat.c
-t s21_cat.c
-b -e s21_cat.c Makefile
-t -e s21_cat.c
-s -v s21_cat.c
-t -n s21_cat.c Makefile
-n -b s21_cat.c
-b -v -s s21_cat.c Makefile
-e -n -s s21_cat.c
-b -t -e s21_cat.c Makefile
-t -v -s s21_cat.c
-e -v -s s21_cat.c Makefile
not_exists.txt
s21_cat.c not_exists.txt
END