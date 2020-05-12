Q = int(input())
queries = []
for i in range(Q):
    l, r = map(int, input().split())
    queries.append((l, r))

N = 10**5 + 1
is_prime = [True] * N
is_prime[0] = False
is_prime[1] = False
for i in range(4, N, 2):
    is_prime[i] = False
for i in range(3, N, 2):
    if i*i > N: break
    if is_prime[i]:
        for j in range(2*i, N, i):
            is_prime[j] = False

def is_ok(x):
    if (x % 2 == 0) or not is_prime[x]:
        return False
    return is_prime[(x + 1) // 2]

cum_ok = [None] * N
c = 0
for i in range(N):
    if is_ok(i):
        c += 1
    cum_ok[i] = c

for l, r in queries:
    print(cum_ok[r] - cum_ok[l-1])
