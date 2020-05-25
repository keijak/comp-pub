#!/usr/bin/env python3
import sys
import collections

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

S = ns()
Q = ni()


def solve():
    ds = collections.deque(S)
    rev = False
    for i in range(Q):
        qt, *q = ns().split()
        if qt == "1":
            rev = not rev
            continue
        assert qt == "2"
        if (q[0] == "1") ^ rev:
            ds.appendleft(q[1])
        else:
            assert (q[0] == "2") ^ rev
            ds.append(q[1])
    if rev:
        ds = reversed(ds)
    return "".join(ds)


print(solve())
