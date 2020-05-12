N, K = map(int, input().split())
X = list(map(int, input().split()))

zeroi = 0
while zeroi < N:
    if X[zeroi] >= 0:
        break
    zeroi += 1
if zeroi < N and X[zeroi] == 0:
    K -= 1
    N -= 1
    del X[zeroi]

xl = list(reversed(X[:zeroi]))
xr = list(X[zeroi:])

def solve():
    if K == 0:
        return 0
    minval = 2**60
    for i, x in enumerate(xl):
        r = K - (i+1)
        if r <= 0:
            d = -x
        elif r > len(xr):
            continue
        else:
            d = -x + 2 * xr[r-1]
        if minval > d:
            minval = d
    for i, x in enumerate(xr):
        l = K - (i+1)
        if l <= 0:
            d = x
        elif l > len(xl):
            continue
        else:
            d = x - 2 * xl[l-1]
        if minval > d:
            minval = d
    return minval

print(solve())