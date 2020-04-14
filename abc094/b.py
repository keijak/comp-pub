n = int(input())
a = list(map(int, input().split()))
m = max(a)

def closest(x):
    res = None
    min_diff = None
    for k in a:
        if k == m:
            continue
        diff = abs(k - x)
        if res is None or min_diff > diff:
            min_diff = diff
            res = k
    return res

print(m, closest(m // 2))
