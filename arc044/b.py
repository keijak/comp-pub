N = int(input())
A = list(map(int, input().split()))
A.sort()
M = 10 ** 9 + 7


def solve():
    if A[0] != 0:
        return 0
    if N > 1 and A[1] != 1:
        return 0
    sp = [0] * N  # number of nodes with distance k from 0.
    sp[0] = 1
    res = 1
    i = 1
    while i < N:
        k = A[i]
        if k != A[i - 1] + 1:
            return 0
        c = 0
        while i < N and A[i] == k:
            i += 1
            c += 1
            res = res * ((pow(2, sp[k - 1], M) - 1) % M) % M
        res = res * pow(2, c * (c - 1) // 2, M) % M
        sp[k] = c
    return res


print(solve())
