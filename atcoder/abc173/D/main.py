import sys
import heapq

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))

N = ini()
A = inl()


def solve():
    A.sort(reverse=True)
    hp = [(-A[1], -A[0]), (-A[1], -A[0])]
    score = A[0]
    for i in range(2, N):
        asmall, abig = heapq.heappop(hp)
        score += -asmall
        heapq.heappush(hp, (-A[i], asmall))
        heapq.heappush(hp, (-A[i], abig))
    return score


print(solve())
