import sys
import collections

ni = lambda: int(sys.stdin.readline())
nl = lambda: list(map(int, sys.stdin.readline().split()))

N = ni()
A = nl()
counter = collections.Counter(A)
total = 0
for n in counter.values():
    total += n * (n - 1) // 2
for i in range(N):
    n = counter[A[i]]
    k = total - (n * (n - 1) // 2) + (n - 1) * (n - 2) // 2
    print(k)
