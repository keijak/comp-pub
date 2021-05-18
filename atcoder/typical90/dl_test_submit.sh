#!/bin/bash

URL="https://atcoder.jp/contests/typical90/tasks/typical90_aq"
CMD="./a.out"
SRC="main.cpp"

while :
do
  date
  atcoder_dl_test_submit.py "$URL" --cmd="$CMD" --src="$SRC" && exit
  echo '.'
  echo '.'
  sleep $(($RANDOM % 5 + 5))
done
