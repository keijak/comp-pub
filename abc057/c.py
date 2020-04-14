from collections import Counter
N = int(input())
n = N
fac = Counter()
while n % 2 == 0:
    n >>= 1
    fac[2] += 1
p = 3
while p*p <= n:
    while n % p == 0:
        n //= p
        fac[p] += 1
    if n == 1:
        break
    p += 2

fac_list = list(fac.items())

def ndigit(x):
    n = 0
    while x > 0:
        n += 1
        x //= 10
    return n

ND = ndigit(N)

def solve(fi, acc):
    if fi == len(fac_list):
        assert N % acc == 0
        other = N // acc
        return max(ndigit(acc), ndigit(other))

    p, max_cnt = fac_list[fi]
    minval = ND
    m = 1
    for i in range(max_cnt+1):
        minval = min(minval, solve(fi+1, acc*m))
        m *= p
    return minval

print(solve(0, 1))
