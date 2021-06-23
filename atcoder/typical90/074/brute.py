import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


n = int(sys.stdin.readline())
dic = {"a": 0, "b": 1, "c": 2}
S = [dic[c] for c in sys.stdin.readline().rstrip()]
# n = len(S)


def f(s):
    if all(c == 0 for c in s):
        return 0
    res = 0
    for i in range(n):
        if s[i] == 0:
            continue
        t = s[:]
        t[i] -= 1
        for j in range(i):
            t[j] = (t[j] + 1) % 3
        res = max(res, f(t) + 1)
    return res


print(f(S))
