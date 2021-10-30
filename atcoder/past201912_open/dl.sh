#!/bin/zsh

set -e

contest="past201912"  # e.g. "past202109"
tasks=(A B C D E F G H I J K L M N O)

if [[ "$contest" = "" ]]; then
    echo "An AtCoder contest name is required."
    exit 1
fi

echo "contest: ${contest}"
echo "tasks: ${tasks}"

for task in $tasks; do
    pushd "${task}"
    oj d "https://atcoder.jp/contests/${contest}-open/tasks/${contest}_${task:l}"
    popd
done
