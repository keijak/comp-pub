a, b, x = map(int, input().split())

if a % x == 0:
    m = a // x
else:
    m = a // x + 1

if b < m*x:
    ans = 0
else:
    ans = (b - m*x) // x + 1

print(ans)