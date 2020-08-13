import sys
import numpy as np

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def convolve(f, g, n):
    fft_len = 1
    alen = 2 * (n + 1)
    while fft_len < alen:
        fft_len *= 2

    fa = np.fft.rfft(f, fft_len)
    fb = np.fft.rfft(g, fft_len)

    h = np.fft.irfft(fa * fb)
    h = np.rint(h).astype(np.int64)
    return h[1 : 2 * n + 1]


def main():
    n = ini()
    a = np.zeros(n + 1, np.int64)
    b = np.zeros(n + 1, np.int64)
    for i in range(n):
        x, y = inl()
        a[i + 1] = x
        b[i + 1] = y
    c = convolve(a, b, n)
    print(*c, sep="\n")


main()
