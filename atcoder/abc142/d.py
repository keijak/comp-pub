from fractions import gcd

a,b = map(int,input().split())
g = gcd(a,b)
facs = [1]
i = 2
while i*i <= g:
    if g == 1:
        break
    if g % i == 0:
        facs.append(i)
        while g % i == 0:
            g //= i
    i += 1
if g != 1:
    facs.append(g)

print(len(facs))
