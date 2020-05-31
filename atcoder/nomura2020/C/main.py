import sys
import itertools

ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())

N = ni()
A = nl()
assert len(A) == (N + 1)


def solve():
    ar = list(itertools.accumulate(A[::-1], initial=0))
    ar.reverse()
    assert len(ar) == (N + 2)
    if N == 0:
        return 1 if A[0] == 1 else -1
    if A[0] != 0:
        return -1
    inn = ans = 1
    for i in range(1, N + 1):
        b = 2 * inn - A[i]
        if b < 0:
            return -1
        inn = min(b, ar[i + 1])
        ans += inn + A[i]
    return ans


print(solve())
