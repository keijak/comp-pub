n = int(input())

if n <= 2:
    print(n, 1, flush=True)
    t = int(input())
    assert t == 0
    exit(0)


rem = n

if n & 1:
    mid = (n + 1) // 2
    print(1, mid, flush=True)
    rem -= 1
else:
    mid = n // 2
    print(2, mid, flush=True)
    rem -= 2
assert rem % 2 == 0

while True:
    t = int(input())
    if t in [0, 1]:
        exit(0)
    k, x = [int(x) for x in input().split()]
    if x < mid:
        y = n + 1 - x
        if k == 2:
            y -= 1
        print(k, y, flush=True)
    else:
        if k == 2:
            x += 1
        y = n + 1 - x
        print(k, y, flush=True)
