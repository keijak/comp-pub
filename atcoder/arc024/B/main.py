import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    n = ini()
    color = [ini() for i in range(n)]
    if all(c == color[0] for c in color):
        return -1

    max_streak = 1
    streak = 1
    for i in range(1, n):
        if color[i] == color[i - 1]:
            streak += 1
        else:
            max_streak = max(max_streak, streak)
            streak = 1

    head = tail = 1
    for i in range(1, n):
        if color[i] != color[i - 1]:
            break
        head += 1
    for i in range(n - 2, -1, -1):
        if color[i] != color[i + 1]:
            break
        tail += 1
    if color[0] == color[-1]:
        max_streak = max(max_streak, head + tail)

    return (max_streak - 1) // 2 + 1


print(solve())
