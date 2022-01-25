#!/bin/zsh

set -e

contest="joi2022yo2"  # e.g. "abc216"
task_prefix="joi2022_yo2"
tasks=(A B C D E)

if [[ "$contest" = "" ]]; then
    echo "An AtCoder contest name is required."
    exit 1
fi

echo "contest: ${contest}"
echo "tasks: ${tasks}"

for task in $tasks; do
    oj dl -d "${task}/test" "https://atcoder.jp/contests/${contest}/tasks/${task_prefix}_${task:l}" &
done
wait
echo "downloaded: ${tasks}"
