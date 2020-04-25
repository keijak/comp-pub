from collections import Counter

M = 10 ** 9 + 7
N = int(input())
A = list(map(int, input().split()))
acnt = Counter(A)


def solve():
    if A[0] != 0 or acnt[0] != 1:
        return 0
    prevcnt = 1
    res = 1
    rest = N - 1
    for k in range(1, N):
        if rest == 0:
            break
        c = acnt[k]
        if c == 0:
            return 0
        rest -= c
        res = res * pow(pow(2, prevcnt, M) - 1, c, M) % M
        res = res * pow(2, c * (c - 1) // 2, M) % M
        prevcnt = c
    return res


print(solve())
