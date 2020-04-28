n = int(input())
h = list(map(int, input().split()))
p0, p1 = 0, abs(h[1] - h[0])
for i in range(2, n):
    p2 = min(p1 + abs(h[i] - h[i - 1]), p0 + abs(h[i] - h[i - 2]))
    p0, p1 = p1, p2
print(p1)
