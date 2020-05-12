N,M = map(int,input().split())
A = list(map(int, input().split()))
s=sum(A)
if N <s:
    print(-1)
else:
    print(N-s)
