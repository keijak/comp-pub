N = int(input())
prefs = []
for i in range(N):
    a, b = map(int, input().split())
    prefs.append((a + b, a, b))
prefs.sort(reverse=True)
ans = 0
for i, (p, a, b) in enumerate(prefs):
    ans += -b if i % 2 else a
print(ans)
