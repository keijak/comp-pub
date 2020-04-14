X, Y = map(int,input().split())

x = X
c = 0
while x <= Y:
    x <<= 1
    c += 1
print(c)