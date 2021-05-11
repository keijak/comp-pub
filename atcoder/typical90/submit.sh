#!/bin/bash

while :
do
  atcoder_dl_test_submit.py https://atcoder.jp/contests/typical90/tasks/typical90_ak --cmd='./a.out' --src='main.cpp'
  echo '.'
  echo '.'
  echo '.'
  sleep $(($RANDOM % 10 + 5))
done
