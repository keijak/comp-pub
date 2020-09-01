import sys
import numpy as np
from collections import Counter

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

p = ini()
n = ini()


def convolve(f, g):
    fft_len = 1
    while 2 * fft_len < len(f) + len(g) - 1:
        fft_len *= 2
    fft_len *= 2

    Ff = np.fft.rfft(f, fft_len)
    Fg = np.fft.rfft(g, fft_len)
    Fh = Ff * Fg
    h = np.fft.irfft(Fh, fft_len)
    h = np.rint(h).astype(np.int64)

    return h[: len(f) + len(g) - 1]


def solve():
    count = np.zeros(p * 2, dtype=np.int64)
    for i in range(p):
        x = pow(i, n, p)
        count[x] += 1

    c = convolve(count, count)
    xy = np.zeros(p, dtype=np.int64)
    for i, k in enumerate(c):
        xy[i % p] += k
    z = count[:p]
    res = xy * z
    # debug(res, res.sum(), len(res))
    return res.sum()


print(solve())
