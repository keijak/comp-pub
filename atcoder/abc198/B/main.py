import sys

try:
    import frosch

    frosch.hook()
except ImportError:
    pass


def debug(*a, **kw):
    print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = sys.stdin.readline().rstrip()
    n = len(s)
    if s.count("0") == n:
        return True
    leading = 0
    while leading < n and s[leading] == "0":
        leading += 1
    trailing = 0
    while trailing < n and s[n - 1 - trailing] == "0":
        trailing += 1
    if trailing < leading:
        return False
    t = s[leading : n - trailing]
    return t == t[::-1]


print(["No", "Yes"][solve()])
