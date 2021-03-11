import sys

debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


def query(x, y):
    print(x, y)
    color = input()
    return color == "white"


n = int(input())
c0 = query(0, 0)
qc = 1
lb = 0  # c0
ub = 10 ** 9  # not c0


while qc < n:
    mid = (lb + ub) // 2
    cq = query(mid, mid)
    qc += 1
    if cq == c0:
        lb = mid
    else:
        ub = mid


x1, y1 = (lb, ub)
x2, y2 = (ub, lb)
print(x1, y1, x2, y2)
