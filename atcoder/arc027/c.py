import copy

X, Y = map(int, input().split())
N = int(input())

tops = []
for i in range(N):
    tickets, happiness = map(int, input().split())
    tops.append((tickets, happiness))

K = min(N + 1, X + 1)
T = X + Y + 1
tp = [0] * (K * T)
for z in range(tops[0][0], T):
    tp[1 * T + z] = tops[0][1]
for i in range(1, N):
    ti = tp.copy()
    nticket, happiness = tops[i]
    for k in range(1, K):
        for z in range(nticket, T):
            ti[k * T + z] = max(
                ti[k * T + z], tp[(k - 1) * T + z - nticket] + happiness
            )
    tp = ti

# print(tp[3 * T : 4 * T])

print(max(tp))
