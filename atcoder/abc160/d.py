N,X,Y = map(int,input().split())
X -= 1
Y -= 1
w = Y - X
dist = [[0] + [1]*(N-1-i) for i in range(N)]

for k in range(Y):
    for r in range(k+1, N):
        old = r - k
        new = abs(X - k) + 1 + abs(Y - r)
        if new < old:
            dist[k][old] -= 1
            dist[k][new] += 1

for i in range(1, N):
    cnt = 0
    for d in dist:
        if i < len(d):
            cnt += d[i]
    print(cnt)
