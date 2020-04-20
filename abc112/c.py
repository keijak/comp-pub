import sys

N = int(input())
obs = []
lowerbound = 1
for i in range(N):
    x, y, h = map(int, input().split())
    obs.append((x, y, h))
    lowerbound = max(lowerbound, h)


def find_height(cx, cy):
    upperbound = 10 ** 12
    summit = None
    for x, y, h in obs:
        s = h + abs(cx - x) + abs(cy - y)
        if s < lowerbound:
            return None
        if h == 0:
            upperbound = min(upperbound, s)
        elif summit is None:
            summit = s
        elif summit != s:
            return None
        if summit is not None and (summit > upperbound or summit < lowerbound):
            return None
    return summit


for cx in range(0, 101):
    for cy in range(0, 101):
        ch = find_height(cx, cy)
        if ch is not None:
            print(cx, cy, ch)
            sys.exit()
