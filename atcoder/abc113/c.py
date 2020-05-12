from collections import defaultdict

N, M = map(int, input().split())
P, Y = [], []
ylist = defaultdict(list)
pset = set()
for i in range(M):
    p, y = map(int, input().split())
    P.append(p)
    Y.append(y)
    ylist[p].append(y)
    pset.add(p)

cdict = defaultdict(dict)
for p in pset:
    syear = sorted(ylist[p])
    for i, y in enumerate(syear):
        cdict[p][y] = i + 1

for i in range(M):
    p = P[i]
    y = Y[i]
    rank = cdict[p][y]
    print('{:06}{:06}'.format(p, rank))
