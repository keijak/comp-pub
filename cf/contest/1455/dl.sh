#!/bin/zsh

set -e

contest="1455"  # e.g. "1455"
tasks=(B C D E F G)

if [[ "$contest" = "" ]]; then
    echo "An AtCoder contest name is required."
    exit 1
fi

echo "contest: ${contest}"
echo "tasks: ${tasks}"

for task in $tasks; do
    pushd "${task}"
    oj d "https://codeforces.com/contest/${contest}/problem/${task}"
    popd
done
