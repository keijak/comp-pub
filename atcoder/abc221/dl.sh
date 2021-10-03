#!/bin/zsh

set -e

contest="abc221"  # e.g. "abc216"
tasks=(A B C D E F G H)

if [[ "$contest" = "" ]]; then
    echo "An AtCoder contest name is required."
    exit 1
fi

echo "contest: ${contest}"
echo "tasks: ${tasks}"

for task in $tasks; do
    pushd "${task}"
    oj d "https://atcoder.jp/contests/${contest}/tasks/${contest}_${task:l}"
    popd
done
