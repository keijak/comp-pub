import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

n, m = inm()
disk = [ini() for _ in range(m)]


def solve():
    box = list(range(1, n + 1))

    cur = 0
    for i in range(m):
        music = disk[i]
        if music == cur:
            continue
        j = box.index(music)
        cur, box[j] = box[j], cur
    return box


print(*solve(), sep="\n")
