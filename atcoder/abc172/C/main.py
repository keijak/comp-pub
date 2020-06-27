from itertools import accumulate
from bisect import bisect

n, m, k = map(int, input().split())
cuma = accumulate(map(int, input().split()), initial=0)
cumb = list(accumulate(map(int, input().split())))
print(max(i + bisect(cumb, k - x) for i, x in enumerate(cuma) if x <= k))
