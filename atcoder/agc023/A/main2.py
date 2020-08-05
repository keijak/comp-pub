import itertools
import collections

N = int(input())
A = list(map(int, input().split()))
acum = list(itertools.accumulate(A, initial=0))
c = collections.Counter(acum)
print(sum(n * (n - 1) // 2 for n in c.values()))  # nC2
