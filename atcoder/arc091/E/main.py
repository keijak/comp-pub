import sys
import collections
import itertools

inm = lambda: map(int, sys.stdin.readline().split())

N, A, B = inm()


def solve():
    if A + B > N + 1:
        return None
    s = [range(B, 0, -1)]
    la = 1
    p = B + 1  # 1-indexed
    ra = N - p + 1
    while la + ra > A:
        p2 = p + min(B, la + ra - A + 1)
        s.append(range(p2 - 1, p - 1, -1))
        p = p2
        la += 1
        ra = N + 1 - p
        if la == A and ra > 0:
            return None
    assert la + ra == A
    s.append(range(p, N + 1))
    return itertools.chain.from_iterable(s)


ans = solve()
if ans is None:
    print(-1)
else:
    print(*ans)
