import sys
N=int(input())
for h in range(3501):
    for n in range(h):
        d = 4*h*n - N*(h+n)
        if d > 0 and N*h*n%d == 0:
            print(h, n, N*h*n//d)
            sys.exit()