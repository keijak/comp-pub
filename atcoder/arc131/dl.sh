#!/bin/zsh

set -e

contest="arc131"  # e.g. "abc216"
tasks=(A B C D E F)

if [[ "$contest" = "" ]]; then
    echo "An AtCoder contest name is required."
    exit 1
fi

echo "contest: ${contest}"
echo "tasks: ${tasks}"

for task in $tasks; do
    oj dl -d "${task}/test" "https://atcoder.jp/contests/${contest}/tasks/${contest}_${task:l}" &
done
wait
echo "downloaded: ${tasks}"
