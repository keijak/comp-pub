S = input()
a, b = 0, 0
for i in range(len(S)):
    d = int(S[i])
    a = 2 * a + b
    b = 10 * b + (2 ** i) * d
print(a + b)
