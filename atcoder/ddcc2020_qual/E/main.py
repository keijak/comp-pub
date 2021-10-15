import sys
from typing import *

n = int(input())
n2 = n * 2


def query(a):
    assert len(a) == n
    print('?', *a, flush=True)
    res = input()
    if res == 'Red':
        return True
    elif res == 'Blue':
        return False
    else:
        exit(-1)


ans: List[Optional[bool]] = [None] * (n2 + 1)


def answer():
    s = ['BR'[x] for x in ans[1:]]
    print('! ' + ''.join(s), flush=True)
    exit(0)


ini = query(list(range(1, n + 1)))
l = 1
r = n + 1
while r - l > 1:
    mid = (r + l) // 2
    res = query(list(range(mid, mid + n)))
    if res == ini:
        l = mid
    else:
        r = mid

ans[l] = ini
ans[l + n] = not ini

for i in range(1, n2 + 1):
    if ans[i] is not None:
        continue
    if l <= i <= l + n:
        continue
    a = list(range(l + 1, l + n))
    a.append(i)
    a.sort()
    res = query(a)
    if res == ini:
        ans[i] = ini
    else:
        ans[i] = not ini

for i in range(1, n2 + 1):
    if ans[i] is not None:
        continue
    a = [i]
    c1, c2 = 0, 0
    for j in range(1, n2 + 1):
        if j == i or ans[j] is None:
            continue
        if ans[j] is True and c1 < n // 2:
            a.append(j)
            c1 += 1
        if ans[j] is False and c2 < n // 2:
            a.append(j)
            c2 += 1
    assert len(a) == n
    a.sort()
    res = query(a)
    if res == ini:
        ans[i] = ini
    else:
        ans[i] = not ini

answer()
