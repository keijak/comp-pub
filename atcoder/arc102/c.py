N, K = map(int, input().split())

def count_k():
    return (N // K) ** 3

def count_khalf():
    if K % 2 == 1:
        return 0
    b = N // K
    if N % K >= K // 2:
        b += 1
    return b ** 3

print(count_k() + count_khalf())
