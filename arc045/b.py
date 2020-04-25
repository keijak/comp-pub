n, m = map(int, input().split())
intervals = []
for i in range(m):
    s, t = map(int, input().split())
    intervals.append((s, t))

deltas = [0] * (n + 2)
for (s, t) in intervals:
    deltas[s] += 1
    deltas[t + 1] -= 1
acum = [0] * (n + 2)
for i in range(1, n + 1):
    acum[i] = acum[i - 1] + deltas[i]

streak_start = [None] * (n + 2)
instreak = False
for i in range(0, n + 2):
    if acum[i] > 1:
        if instreak:
            streak_start[i] = streak_start[i - 1]
        else:
            streak_start[i] = i
            instreak = True
    else:
        instreak = False

redundants = []
for i, (s, t) in enumerate(intervals):
    ss, st = streak_start[s], streak_start[t]
    if ss is not None and st is not None and ss == st:
        redundants.append(i + 1)

print(len(redundants))
for r in redundants:
    print(r)
