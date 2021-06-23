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
    count = 0
    while True:
        updated = False
        for i in range(n):
            if s[i] > 0:
                count += 1
                s[i] -= 1
                for j in range(i):
                    s[j] = (s[j] + 1) % 3
                updated = True
                break
        if not updated:
            break
    return count


print(f(S))
