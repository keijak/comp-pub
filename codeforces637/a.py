N = int(input())
for i in range(N):
    n, a, b, c, d = map(int, input().split())
    wmin, wmax = n * (a - b), n * (a + b)
    if wmax < c - d or wmin > c + d:
        print("No")
    else:
        print("Yes")
