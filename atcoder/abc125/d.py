N = int(input())
A = list(map(int,input().split()))
assert len(A) == N

def solve():
    negcnt = len([a for a in A if a<0])
    aa = [abs(a) for a in A]
    if negcnt % 2 == 0:
        return sum(aa)
    mv = min(aa)
    return sum(aa) -2*mv
print(solve())
