def solve():
    d, n = map(int, input().split())
    if n == 100:
        n = 101
    if d == 0:
        return n
    if d == 1:
        return n * 100
    return n * 10000


print(solve())
