import sys
import numpy as np

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

P = 200003
PROOT = 2


def calc_tables():
    prev = [None] * P
    val = 1
    for i in range(P):
        prev[val] = i
        val = (val * PROOT) % P
    return prev


def convolve(f, g):
    fft_len = 1
    alen = len(f) + len(g) - 1
    while fft_len < alen:
        fft_len *= 2

    fa = np.fft.rfft(f, fft_len)
    fb = np.fft.rfft(g, fft_len)

    h = np.fft.irfft(fa * fb)
    h = np.rint(h).astype(np.int64)
    return h


def solve():
    prev = calc_tables()

    _ = ini()
    a = [x for x in inl() if x != 0]
    b = [prev[x] for x in a]
    a = np.array(a, dtype=np.int64)
    m = len(a)

    b_counts = np.zeros(P, np.int64)
    for x in b:
        b_counts[x] += 1

    root_pow = np.ones(2 * P, dtype=np.int64)
    for i in range(1, 2 * P):
        root_pow[i] = root_pow[i - 1] * PROOT % P

    c = convolve(b_counts, b_counts)
    ans = 0
    for k in range(2 * P - 1):
        ans += c[k] * root_pow[k]
    for k in range(P):
        ans -= b_counts[k] * root_pow[2 * k]
    return ans // 2


print(solve())
