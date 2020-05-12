import itertools
N = int(input())
A = list(map(int,input().split()))
A.sort()
acum = list(itertools.accumulate(A))

def solve():
    unreachable = None
    for i in range(N-2,-1,-1):
        if 2*acum[i] < A[i+1]:
            unreachable = i
            break
    if unreachable is None:
        return N
    return N - 1 - unreachable

print(solve())