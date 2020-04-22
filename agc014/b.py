from collections import Counter

N, M = map(int, input().split())
counter = Counter()
for i in range(M):
    a, b = map(int, input().split())
    counter[a] += 1
    counter[b] += 1

if all(x % 2 == 0 for x in counter.values()):
    print("YES")
else:
    print("NO")
