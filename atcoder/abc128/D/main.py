import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N, K = nm()
V = nl()
assert len(V) == N
INF = 10 ** 9


def solve():
    ans = 0
    for i in range(K + 1):
        if i > N:
            break
        for j in range(K + 1 - i):
            if i + j > N:
                break
            v = V[:i] + V[N - j :]
            v.sort()
            s = sum(v)
            k = 0
            while k < K - i - j:
                if k >= len(v):
                    break
                if v[k] >= 0:
                    break
                s -= v[k]
                k += 1
            if ans < s:
                ans = s
                # print(f"> ans={ans} i={i}, j={j}, k={k}", file=sys.stderr)
    return ans


print(solve())
