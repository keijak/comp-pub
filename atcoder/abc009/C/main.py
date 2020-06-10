import sys
import collections

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N, K = inm()
S = list(ins())
assert len(S) == N
OA = ord("a")


def solve():
    tc = collections.Counter(S)
    sc = collections.Counter(S)
    prefix_unmatch = 0
    T = [None] * N
    for i in range(N):
        next_char = None
        sc[S[i]] -= 1
        for j in range(26):
            c = chr(OA + j)
            if not tc[c]:
                continue
            tc[c] -= 1
            suffix_unmatch = 0
            for k in range(26):
                d = chr(OA + k)
                suffix_unmatch += abs(sc[d] - tc[d])
            suffix_unmatch //= 2
            if prefix_unmatch + int(S[i] != c) + suffix_unmatch <= K:
                next_char = c
                break
            tc[c] += 1
        assert next_char is not None
        T[i] = next_char
        if S[i] != next_char:
            prefix_unmatch += 1
    return T


print("".join(solve()))
