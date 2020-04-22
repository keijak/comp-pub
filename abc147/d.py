M = 10 ** 9 + 7
N = int(input())
A = list(map(int, input().split()))
pow2 = [2 ** i % M for i in range(60)]
ans = 0
a1 = [(A[0] >> i) & 1 for i in range(60)]
a0 = [1 - x for x in a1]
for i in range(1, N):
    a = A[i]
    for j in range(60):
        if (a >> j) & 1:
            ans += pow2[j] * a0[j] % M
            a1[j] += 1
        else:
            ans += pow2[j] * a1[j] % M
            a0[j] += 1
    ans %= M
print(ans)
