from collections import defaultdict

BIG = 10000

h, w = map(int,input().split())

s = ['']*h
for i in range(h):
    s[i] = input()

cost = [[0]*w]*h

if s[0][0] == '#':
    cost[0][0] = 1

for r in range(h):
    for c in range(w):
        if (r, c) == (0, 0): continue
        x = BIG
        if s[r][c] == '.':
            if r > 0:
                x = min(x, cost[r-1][c])
            if c > 0:
                x = min(x, cost[r][c-1])
        else:  # black
            if r > 0:
                if s[r-1][c] == '#':
                    x = min(x, cost[r-1][c])
                else:
                    x = min(x, cost[r-1][c]+1)
            if c > 0:
                if s[r][c-1] == '#':
                    x = min(x, cost[r][c-1])
                else:
                    x = min(x, cost[r][c-1]+1)
        cost[r][c] = x

print(cost[h-1][w-1])