T = int(input())
for _ in range(T):
    n, k = map(int, input().split())
    hs = list(map(int, input().split()))
    assert len(hs) == n
    peak = [False] * n
    for i in range(1, n - 1):
        if hs[i] > hs[i - 1] and hs[i] > hs[i + 1]:
            peak[i] = True
    apeak = [0] * n
    for i in range(1, n):
        apeak[i] = apeak[i - 1] + peak[i]
    maxp = -1
    maxr = None
    for i in range(k - 2, n):
        p = apeak[i] - apeak[i - k + 2]
        if maxp < p:
            maxp = p
            maxr = i
    # print(f"n={n} k={k} l={maxr+3-k} r={maxr} apeak={apeak}")
    print(maxp + 1, maxr + 3 - k)
