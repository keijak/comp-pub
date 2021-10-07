#!/bin/zsh

set -e

contest="past202104-open"  # e.g. "abc216"
tasks=(A B C D E F G H I J K L N O)

if [[ "$contest" = "" ]]; then
    echo "An AtCoder contest name is required."
    exit 1
fi

echo "contest: ${contest}"
echo "tasks: ${tasks}"

for task in $tasks; do
    pushd "${task}"
    oj d "https://atcoder.jp/contests/${contest}/tasks/past202104_${task:l}"
    popd
done
