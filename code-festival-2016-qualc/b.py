K, T = map(int, input().split())
A = list(map(int, input().split()))
assert len(A) == T
amax = max(A)
rest = sum(A) - amax
print(max(amax - rest - 1, 0))
