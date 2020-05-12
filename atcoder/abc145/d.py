M = 10**9 + 7
x,y = map(int, input().split())

def solve():
    at = 2*x - y
    bt = 2*y - x
    if at < 0 or bt < 0 or at % 3 != 0 or bt %3 != 0:
        return 0
    a = at // 3
    b = bt // 3
    num = den = 1
    for i in range(max(a, b)+1, (a+b)+1):
        num = num * i % M
    for i in range(2, min(a, b)+1):
        den = den * i % M
    inv_den = pow(den, M-2, M)
    return num * inv_den % M

print(solve())
