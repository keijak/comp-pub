from collections import Counter

M = 998244353
N = int(input())
D = list(map(int, input().split()))
assert len(D) == N
dcnt = Counter(D)

def solve():
    if D[0] != 0:
        return 0
    D.sort()
    if N > 1 and D[1] != 1:
        return 0
    i = 1
    res = 1
    leaf = 1
    k = 0
    while i < N:
        if D[i] != k + 1:
            return 0
        k += 1
        c = dcnt[k]
        res *= pow(leaf, c, M)
        leaf = c
        i += c
    return res % M

if __name__ == "__main__":
    print(solve())
