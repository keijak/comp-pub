#!/bin/zsh

set -e

contest="past202112-open"
tasks=(A B C D E F G H I J K L M N O)
task_prefix="past202112"

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
