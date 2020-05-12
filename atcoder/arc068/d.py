import collections
N = int(input())
A = map(int, input().split())
c = collections.Counter(A)

neven = nodd = 0
for v in c.values():
    if v % 2 == 1:
        nodd += 1
    else:
        neven += 1

if neven % 2 == 0:
    ans = nodd + neven
else:
    ans = nodd + neven - 1
print(ans)
