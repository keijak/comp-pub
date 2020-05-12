N = int(input())
A = list(map(int, input().split()))

bit_index = [-1] * 20
cnt = 0
pos = -1
for i in range(N):
    a = A[i]
    for j in range(20):
        if a & 1:
            pos = max(pos, bit_index[j])
            bit_index[j] = i
        a >>= 1
        if not a:
            break
    cnt += i - pos
print(cnt)
