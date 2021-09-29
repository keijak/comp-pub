import sys

n, X = open(0).read().split()
n = int(n)
x = int(X, 2)
# print(n, X, x)

lis = [bin(i) for i in range(1, 2 ** n)]
lis.sort()

print(n, X, x, file=sys.stderr)
for i, s in enumerate(lis):
    print(i, s, file=sys.stderr)

print(lis[x - 1][2:])
