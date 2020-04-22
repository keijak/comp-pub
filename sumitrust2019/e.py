M = 1000000007
N = int(input())
left = list(map(int, input().split()))


def solve():
    res = 1
    h = [0, 0, 0]
    for i in range(N):
        k = h.count(left[i])
        res = res * k % M
        for j in range(3):
            if h[j] == left[i]:
                h[j] += 1
                break
    return res


print(solve())
