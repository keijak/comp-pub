from collections import Counter

H, W = map(int, input().split())
mat = [input() for _ in range(H)]
cnts = Counter(c for r in mat for c in r)
odd = 0
if H % 2:
    odd += 1
if W % 2:
    odd += 1


def solve():
    v1 = list(cnts.values())
    rest = sum(v1)
    assert rest == H * W
    v2, v3 = [], []
    for v in v1:
        rest -= v - v % 4
        if v % 4:
            v2.append(v % 4)
    if odd == 0:
        return rest == 0 and not v2
    if H % 2 and W % 2 == 0 and rest > W:
        return False
    if H % 2 == 0 and W % 2 and rest > H:
        return False
    if H % 2 and W % 2 and rest > H + W - 1:
        return False
    for v in v2:
        rest -= v - v % 2
        if v % 2:
            v3.append(v % 2)
    if odd == 1:
        return rest == 0 and not v3
    return rest == 1 and len(v3) == 1


print("Yes" if solve() else "No")
