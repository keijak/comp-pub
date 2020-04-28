import copy

X, Y = map(int, input().split())
N = int(input())

tops = []
for i in range(N):
    tickets, happiness = map(int, input().split())
    tops.append((tickets, happiness))

tp = [0] * ((Y + 1) * (X + 1))
for x in range(1, X + 1):
    for y in range(Y + 1):
        if x + y >= tops[0][0]:
            tp[x * (Y + 1) + y] = tops[0][1]
for i in range(1, N):
    ti = copy.deepcopy(tp)
    nticket, happiness = tops[i]
    for x in range(1, X + 1):
        y = nticket - x
        if y < 0 or y > Y:
            continue
        for j in range(x, X + 1):
            for k in range(y, Y + 1):
                ti[j * (Y + 1) + k] = max(
                    ti[j * (Y + 1) + k], tp[(j - x) * (Y + 1) + (k - y)] + happiness
                )
    tp = ti

print(tp[X * (Y + 1) + Y])
