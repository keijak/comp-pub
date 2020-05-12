n = int(input())
UPPER = 10**18

def solve(n):
    if n%2 == 1:
        return 0
    res = 0
    fiv = 2*5
    while fiv <= n:
        res += n // fiv
        fiv *= 5
    return res

print(solve(n))