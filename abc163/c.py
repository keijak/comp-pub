import collections
N = int(input())
boss = list(map(int, input().split()))
c = collections.Counter()
for b in boss:
    c[b] += 1
for i in range(1,N+1):
    print(c[i])
