N = int(input())
A = [int(x) for x in input().split()]

def solve():
    B = [None] * N
    B[0] = sum(A[i] * (-1 if i%2==1 else 1) for i in range(N))
    for i in range(N-1, 0, -1):
        B[i] = 2 * A[i] - B[(i+1) % N]
    return B

print(*solve())