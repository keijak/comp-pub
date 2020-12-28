import sys
import numpy as np

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def plot(a, px, py, out):
    c = np.array([px, py], dtype="int64")
    return out.extend(x + c for x in a)


def solve():
    ans = [np.array([0, 10 ** 4], dtype="int64")]
    x = 1
    offset = 1
    while 10 * x > len(ans):
        next_ans = list(ans)
        plot(ans, offset, 0, next_ans)
        plot(ans, 0, -offset, next_ans)
        ans = next_ans
        offset *= 3
        x *= 2
    return ans


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
a = solve()
print(len(a))
for x, y in a:
    print(x, y)
