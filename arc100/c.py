N = int(input())
A = list(map(int,input().split()))
B = sorted(A[i] - i - 1 for i in range(N))
def sad(b):
    return sum(abs(A[i] - i - 1 - b)for i in range(N))
print(min(sad(B[N//2]),sad(B[min(N//2+1,N-1)])))
