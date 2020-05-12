import numpy as np

X, Y = map(int, input().split())
N = int(input())

tops = []
for i in range(N):
    tickets, happiness = map(int, input().split())
    tops.append((tickets, happiness))

K = min(N + 1, X + 1)
T = X + Y + 1
tp = np.zeros((K, T), dtype=np.int64)
for nticket, happiness in tops:
    np.maximum(tp[1:, nticket:], tp[:-1, :-nticket] + happiness, out=tp[1:, nticket:])
print(tp.max())
