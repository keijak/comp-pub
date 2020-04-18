N = int(input())
t = list(range(N+1))
for i in range(2, N+1):
    x = t[i-1] + 1
    p6, p9 = 6, 9
    while i-p6 >= 0:
        x = min(x, t[i-p6] + 1)
        p6 *= 6
    while i-p9 >= 0:
        x = min(x, t[i-p9] + 1)
        p9 *= 9
    t[i] = x
print(t[N])
