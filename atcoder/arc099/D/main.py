import sys
from collections import deque
from fractions import Fraction

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

SMAX = 10 ** 15
K = ini()


def dsum(x):
    s = 0
    while x:
        s += x % 10
        x //= 10
    return s


def get_frac(n):
    return Fraction(n, dsum(n))


def solve():
    ans = deque()
    cur = SMAX - 1
    min_frac = get_frac(cur)
    ans.appendleft(cur)
    next_digits = 13
    while next_digits >= 1:
        next_nines = 10 ** next_digits - 1
        counter_base = 1
        while cur - counter_base >= next_nines:
            next_frac = get_frac(cur - counter_base)
            if next_frac > min_frac:
                counter_base *= 10
                continue
            min_frac = next_frac
            cur = cur - counter_base
            ans.appendleft(cur)
            counter_base = 1
        cur = next_nines
        next_digits -= 1
    for x in range(8, 0, -1):
        ans.appendleft(x)
    return ans


ans = solve()
for i, x in enumerate(ans[:K]):
    print(x)
