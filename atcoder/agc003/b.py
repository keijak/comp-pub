N = int(input())
A = [int(input()) for _ in range(N)]

ans = 0
carry = 0
for a in A:
    if a == 0:
        carry = 0
        continue
    ans += (a + carry) // 2
    carry = (a + carry) % 2
print(ans)
