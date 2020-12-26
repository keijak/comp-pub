a, b = map(int, input().split())
r = (b-1) // (a-1)
if (b-1) % (a-1) != 0:
    r += 1
print(r)
