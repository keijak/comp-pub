import collections

N = int(input())
balls = []
for i in range(N):
    x, y = map(int, input().split())
    balls.append((x, y))

moves = collections.defaultdict(list)
mmax = 0
vmax = None
for i in range(N):
    for j in range(i):
        x1, y1 = balls[i]
        x2, y2 = balls[j]
        if y1 > y2 or (y1 == y2 and x1 > x2):
            x1, x2 = x2, x1
            y1, y2 = y2, y1
        moves[x2-x1, y2-y1].append((j, i))
        m = len(moves[x2-x1, y2-y1])
        if m > mmax:
            mmax = m
            vmax = (x2-x1, y2-y1)

print(N - mmax)
