# Two pointers (尺取り法)
N = int(input())
A = list(map(int, input().split()))
cnt = 0
r = 1
mask = A[0]
for i, a in enumerate(A):
    while r < N and (A[r] & mask) == 0:
        mask |= A[r]
        r += 1
    cnt += r - i
    mask &= ~a
print(cnt)
