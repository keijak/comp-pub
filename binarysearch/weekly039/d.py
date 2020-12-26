import sys

ini = lambda: int(sys.stdin.readline())
inl = lambda: [int(x) for x in sys.stdin.readline().split()]
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

from collections import Counter

INF = 10 ** 6


class Solution:
    def solve(self, s):
        offset = ord("a")
        counter = [-1] * 26
        n = len(s)
        for i in range(n):
            c = ord(s[i]) - offset
            if c == 0:
                counter[c] = i
            elif counter[c - 1] >= 0:
                counter[c] = counter[c - 1]
                if c == 26:
                    return i - counter[c]
        return -1


try:
    import frosch

    frosch.hook()
except ImportError:
    pass

s = Solution()
st = "abcdefghijklmnopqrstuvwxyz"  # expect 26
st = "zyxwvutsrqponmlkjihgfedcba"  # expect 26
print(s.solve(st))
