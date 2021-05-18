#!/bin/bash

URL="https://atcoder.jp/contests/typical90/tasks/typical90_aq"
SRC="main.cpp"

while :
do
  date
  if [[ -e DONE ]]; then
    echo 'Already done.'
    exit
  else
    oj submit --yes --wait=0 "$URL" "$SRC" && echo "ok" > DONE
  fi
  echo '.'
  echo '.'
  sleep $(($RANDOM % 5 + 5))
done
