N=int(input())
A=list(map(int,input().split()))

def check():
    balls = [None] * N  # 0 or 1
    for i in range(N-1,-1,-1):
        k = i+1
        if 2*k > N:
            balls[i] = A[i]
            continue
        r = 0
        for j in range(2*k-1,N,k):
            r = (r + balls[j]) & 1
        balls[i] = r ^ A[i]
    return [i+1 for i, b in enumerate(balls) if b]

c = check()
if c is None:
    print(-1)
else:
    print(len(c))
    if c:
        print(*c)