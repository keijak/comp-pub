from fractions import gcd
N,K = map(int, input().split())
A = list(map(int, input().split()))
A.sort()

g = A[0]
for i in range(1, N):
    g = gcd(g, A[i])

def solve():
    return K <= A[-1] and K % g == 0

print('POSSIBLE' if solve() else 'IMPOSSIBLE')
