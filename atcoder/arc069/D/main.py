import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
S = ins()


def flip(x):
    if x == "S":
        return "W"
    else:
        return "S"


def solve():
    for p in ["SS", "SW", "WS", "WW"]:
        buf = list(p)
        for i in range(1, N):
            if (buf[i] == "S" and S[i] == "o") or (buf[i] == "W" and S[i] == "x"):
                buf.append(buf[i - 1])
            else:
                buf.append(flip(buf[i - 1]))
        assert len(buf) == N + 1
        if buf[0] != buf[-1]:
            continue
        buf.pop()
        if buf[1] == buf[-1]:
            if (buf[0] == "S" and S[0] == "x") or (buf[0] == "W" and S[0] == "o"):
                continue
        else:
            if (buf[0] == "S" and S[0] == "o") or (buf[0] == "W" and S[0] == "x"):
                continue
        return "".join(buf)
    return -1


print(solve())
