h, w, a, b = map(int, input().split())

for _ in range(b):
    print("0" * a + "1" * (w - a))
for _ in range(b, h):
    print("1" * a + "0" * (w - a))
