#!/bin/bash

while :
do
  atcoder_dl_test_submit.py https://atcoder.jp/contests/typical90/tasks/typical90_ai --cmd='./a.out' --src='main.cpp'
  echo '.'
  sleep 30
done
