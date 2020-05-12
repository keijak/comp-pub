n,m = map(int,input().split())
a = list(map(int,input().split()))
assert len(a) == n
cb = [None]*m
for i in range(m):
    b, c = map(int,input().split())
    cb[i] = (c,b)
a.sort(reverse=True)
cb.sort(reverse=True)

def solve():
    ai = 0
    ci = 0
    res = [None]*n
    for i in range(n):
        if ci >= len(cb):
            cx, bx = -100, 1
        else:
            cx, bx = cb[ci]
        ax = a[ai]
        if ax >= cx:
            res[i] = ax
            ai += 1
        else:
            res[i] = cx
            cb[ci] = (cx, bx-1)
            if bx-1 <= 0:
                ci += 1
    return sum(res)

print(solve())
