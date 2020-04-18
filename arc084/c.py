import bisect
N = int(input())
A = list(map(int, input().split()))
B = list(map(int, input().split()))
C = list(map(int, input().split()))
A.sort()
B.sort()
C.sort()

bc = [None]*(N+1)
bc[N] = 0
for i in range(N-1, -1, -1):
    bc[i] = bc[i+1] + N - bisect.bisect_right(C, B[i])
ans = 0
for i in range(N):
    ans += bc[bisect.bisect_right(B, A[i])]
print(ans)
