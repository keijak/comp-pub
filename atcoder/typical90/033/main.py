h, w = map(int, input().split())
if h > 1 and w > 1:
    h = (h + 1) // 2
    w = (w + 1) // 2
print(h * w)
