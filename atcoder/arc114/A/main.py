import sys
import math

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]
m = len(primes)


def solve():
    n = int(sys.stdin.readline())
    X = [int(x) for x in sys.stdin.readline().split()]
    ans = []
    for q in range(1, 1 << m):
        y = 1
        for i in range(m):
            if (q >> i) & 1:
                y *= primes[i]
        ok = True
        for x in X:
            if math.gcd(x, y) == 1:
                ok = False
                break
        if ok:
            ans.append(y)
    return min(ans)


print(solve())
