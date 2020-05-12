import sys

n = int(input())
a = [0]*n
for i in range(n):
    a[i] = int(input()) - 1

step = 0
t = 0
seen = {0}
while True:
    t = a[t]
    step += 1
    if t == 1:
        print(step)
        sys.exit()
    if t in seen:
        print(-1)
        sys.exit()
    seen.add(t)
