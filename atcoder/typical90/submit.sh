#!/bin/bash

while :
do
  date
  atcoder_dl_test_submit.py https://atcoder.jp/contests/typical90/tasks/typical90_am --cmd='./a.out' --src='main.cpp'
  echo '.'
  echo '.'
  sleep $(($RANDOM % 5 + 5))
done
