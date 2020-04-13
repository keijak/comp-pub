N, K = map(int,input().split())
A = [int(x) for x in input().split()]
assert len(A) == N


def incr_right(left, right, asum):
    while asum < K:
        if right >= N:
            return False, None, None
        asum += A[right]
        right += 1
    return True, right, asum


def solve():
    res = 0
    left, right = 0, 0
    asum = 0
    for left in range(N):
        if asum < K:
            found, right, asum = incr_right(left, right, asum)
            if not found:
                break
        res += N - right + 1
        asum -= A[left]
    return res


print(solve())