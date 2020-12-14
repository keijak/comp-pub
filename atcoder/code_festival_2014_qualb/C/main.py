import sys
from collections import Counter

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s1 = ins()
    n = len(s1) // 2
    s1 = Counter(s1)
    s2 = Counter(ins())
    s3 = Counter(ins())
    buf = Counter()
    m2 = 0
    for ch, cnt in s3.items():
        k = min(s2[ch], cnt)
        m2 += k
        s3[ch] -= k
        buf[ch] += k
    if m2 < n:
        return False
    m1 = 0
    for ch, cnt in s3.items():
        if cnt <= 0:
            continue
        if s1[ch] < cnt:
            return False
        k = cnt
        s1[ch] -= k
        s3[ch] = 0
        m1 += k
    if m1 + m2 != 2 * n:
        return False
    for ch, cnt in buf.items():
        if m1 == n:
            break
        k = min(s1[ch], cnt, n - m1)
        buf[ch] -= k
        s1[ch] -= k
        m1 += k
        m2 -= k

    return m1 == n and m2 == n


if __name__ == "__main__":
    try:
        import frosch

        frosch.hook()
    except ImportError:
        pass
    print("YES" if solve() else "NO")
