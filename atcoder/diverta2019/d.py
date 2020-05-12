N = int(input())

def solve():
    ans = 0
    for i in range(1, N):
        m = (N - 1) // i
        if i >= m:
            break
        if (N - i) % i == 0:
            ans += (N - i) // i
    return ans

print(solve())
