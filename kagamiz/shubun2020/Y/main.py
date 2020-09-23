a, b = map(int, input().split())
print(eval("".join(map(chr, [97, 47, 47, 98])), {"a": a, "b": b}))
