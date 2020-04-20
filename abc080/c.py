N = int(input())
F = []
for i in range(N):
    F.append(list(map(int, input().split())))
P = []
for i in range(N):
    P.append(list(map(int, input().split())))

maxprofit = -(10 ** 10)
for i in range(1, 2 ** 10):
    conflicts = [0] * N
    for t in range(10):
        opening = bool(i & (1 << t))
        for j in range(N):
            if F[j][t] and opening:
                conflicts[j] += 1
    profit = 0
    for j in range(N):
        profit += P[j][conflicts[j]]
    if maxprofit < profit:
        maxprofit = profit
        maxstate = i

print(maxprofit)
# print(bin(maxstate))
