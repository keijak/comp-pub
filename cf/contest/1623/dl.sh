#!/bin/zsh

set -e

contest="1623"
tasks=(A B C D E)

if [[ "$contest" = "" ]]; then
    echo "A CodeForces contest ID is required."
    exit 1
fi

echo "contest: ${contest}"
echo "tasks: ${tasks}"

for task in $tasks; do
    mkdir -p "${task}"
    pushd "${task}"
    oj d "https://codeforces.com/contest/${contest}/problem/${task}"
    popd
done
