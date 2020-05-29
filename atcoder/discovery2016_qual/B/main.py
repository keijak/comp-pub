import sys
import collections

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N = ni()
A = nl()
assert len(A) == N


def solve():
    mx = max(A)
    vals = collections.defaultdict(list)
    for i, x in enumerate(A):
        if i == 0 and x == mx:
            vals[x].append(N)
        else:
            vals[x].append(i)
    keys = sorted(vals.keys())
    for k in keys:
        vals[k].sort()
    cur, cnt = -1, 0
    for k in keys:
        ixs = vals[k]
        li = ixs[-1]
        while ixs:
            if ixs[-1] < cur:
                break
            ixs.pop()
        if ixs:
            cnt += 1
            cur = ixs[-1]
        else:
            cur = li
        # print(f"x={x} cur={cur} i={i} cnt={cnt}", file=sys.stderr)
    cnt += 1
    return cnt


print(solve())
