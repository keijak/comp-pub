import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

S = ins()
n = len(S)


def parse_int(i):
    if i >= n:
        return False, i
    if S[i] not in "0123456789":
        return False, i
    while i < n and S[i] in "0123456789":
        i += 1
    return True, i


def parse_set(i):
    if i >= n:
        return False, i
    if S[i] != "{":
        return False, i
    i += 1
    while i < n:
        ok, i = parse_literal(i)
        if not ok or i >= n:
            return False, i
        if S[i] == "}":
            i += 1
            return True, i
        if S[i] != ",":
            return False, i
        i += 1
    return False, i


def parse_dict(i):
    if i >= n:
        return False, i
    if S[i] != "{":
        return False, i
    i += 1

    if S[i] == "}":
        i += 1
        return True, i

    while i < n:
        ok, i = parse_literal(i)
        if not ok or i >= n:
            return False, i
        if S[i] != ":":
            return False, i
        i += 1
        ok, i = parse_literal(i)
        if not ok or i >= n:
            return False, i
        if S[i] == "}":
            i += 1
            return True, i
        if S[i] != ",":
            return False, i
        i += 1
    return False, i


def parse_literal(i):
    ok, j = parse_int(i)
    if ok:
        return True, j
    ok, j = parse_set(i)
    if ok:
        return True, j
    return parse_dict(i)


def solve():
    ok, j = parse_dict(0)
    if ok and j == n:
        return "dict"
    else:
        return "set"


print(solve())
