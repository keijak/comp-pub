N, K = map(int, input().split())
print((N - 1) // (K - 1) + int((N - 1) % (K - 1) != 0))
