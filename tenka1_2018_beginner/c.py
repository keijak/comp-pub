import collections
N = int(input())
A = []
for i in range(N):
    A.append(int(input()))
A.sort()

s = collections.deque([A[0]])
i = 1
j = N-1
while i <= j:
    xl = max(abs(s[0]-A[j]), abs(s[-1]-A[j]))
    xs = max(abs(s[0]-A[i]), abs(s[-1]-A[i]))
    if xl > xs:
        x = A[j]
        j -= 1
    else:
        x = A[i]
        i += 1
    if abs(s[0]-x) > abs(s[-1]-x):
        s.appendleft(x)
    else:
        s.append(x)
assert len(s) == N
ans = 0
for i in range(1, N):
    ans += abs(s[i] - s[i-1])
print(ans)
