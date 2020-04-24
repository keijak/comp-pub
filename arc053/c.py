import itertools

N = int(input())
downs, ups = [], []
for i in range(N):
    a, b = map(int, input().split())
    if a < b:
        downs.append((a, b))
    else:
        ups.append((a, b))
downs.sort(key=lambda x: (x[0], x[0] - x[1]))
ups.sort(key=lambda x: (x[1], x[1] - x[0]), reverse=True)
curh, maxh = 0, 0
for i, (a, b) in enumerate(itertools.chain(downs, ups)):
    maxh = max(curh + a, maxh)
    curh = curh + a - b
print(maxh)
