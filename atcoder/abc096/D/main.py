N = int(input())
ans = []
for p in range(7, 55555, 10):
    pr, j = True, 3
    while j * j <= p:
        if p % j == 0:
            pr = False
            break
        j += 2
    if pr:
        ans.append(p)
        if len(ans) == N:
            break
print(*ans)
