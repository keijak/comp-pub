a1, a2, a3 = map(int, input().split())
if a1 > a3:
    a1, a3 = a3, a1
ans = 0
if a2 < a1:
    ans += a1 - a2
    a2 = a1
if a2 > a3:
    ans += a2 - a3
    a3 = a2

d = abs(abs(a2 - a1) - abs(a3 - a2))
if a3 - a2 > a2 - a1:
    ans += (d + 1) // 2 + d % 2
elif a3 - a2 < a2 - a1:
    ans += d
print(ans)
