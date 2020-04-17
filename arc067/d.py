n, a, b = map(int, input().split())
x = list(map(int, input().split()))

psum = 0
for i in range(1, n):
    dist = x[i] - x[i-1]
    psum += min(dist*a, b)
print(psum)
