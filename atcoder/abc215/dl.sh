#!/bin/zsh

set -e

contest=abc215

for task in A B C D E F G H; do
    pushd "${task}"
    oj d "https://atcoder.jp/contests/${contest}/tasks/${contest}_${task:l}"
    popd
done
