N = int(input())
A = list(map(int, input().split()))
B = list(map(int, input().split()))

def solve():
    delta = sum(B) - sum(A)
    if delta < 0:
        return False
    aup = [0] * N
    bup = [0] * N
    for i in range(N):
        d = B[i] - A[i]
        if d > 0:
            aup[i] = d // 2 + d % 2
            bup[i] = d % 2
        elif d < 0:
            bup[i] = -d
    adelta = delta - sum(aup)
    bdelta = delta - sum(bup)
    return adelta >= 0 and bdelta >= 0 and 2 * adelta == bdelta

print('Yes' if solve() else 'No')
