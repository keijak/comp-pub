import sys

f = sys.stdin
n = int(f.readline())
a, b = map(int, f.readline().split())
P = list(map(int, f.readline().split()))
x = [0 for _ in range(3)]
for p in P:
    if p <= a:
        x[0] += 1
    elif p <= b:
        x[1] += 1
    else:
        x[2] += 1
print(min(x))
