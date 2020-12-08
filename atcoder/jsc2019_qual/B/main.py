from bisect import bisect_left

n, k = map(int,input().split())
a = list(map(int,input().split()))
assert len(a) == n
sa = sorted(a)
M = 10**9 + 7

inv_before = [0] * n
for i in range(n):
    for j in range(i+1, n):
        if a[i] > a[j]:
            inv_before[j] += 1

inv_all = [0] * n
for i in range(n):
    inv_all[i] = bisect_left(sa, a[i])

def solve():
    ans = 0
    for i in range(n):
        ans += (k * inv_before[i]) % M
        ans %= M
        ans += (k * (k-1) // 2 * inv_all[i]) % M
        ans %= M
    return ans

print(solve())
