import sys
import re

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def solve():
    s = ins()
    st = set()
    for u in re.findall(r"@([a-z]+)", s):
        st.add(u)
    return sorted(st)


try:
    import frosch

    frosch.hook()
except ImportError:
    pass
print(*solve(), sep="\n")
