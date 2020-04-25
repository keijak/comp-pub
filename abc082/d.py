import sys

sys.setrecursionlimit(10 ** 9)
input = sys.stdin.readline
s = input().rstrip()
gx, gy = map(int, input().split())
rx, ry = 0, 0
head, inst = 0, []
while head < len(s) and s[head] == "F":
    head += 1
    rx += 1

xlist, ylist = [], []
heading_x = True
while head < len(s):
    tcnt = 0
    while head < len(s) and s[head] == "T":
        tcnt += 1
        head += 1
    if head == len(s):
        break
    fcnt = 0
    while head < len(s) and s[head] == "F":
        fcnt += 1
        head += 1
    if ((tcnt % 2) == 1) ^ heading_x:
        heading_x = True
        xlist.append(fcnt)
    else:
        heading_x = False
        ylist.append(fcnt)


def fill(fs, goal_offset):
    """Knapsack"""
    f_total = sum(fs)
    if f_total < goal_offset:
        return False
    if f_total == goal_offset:
        return True
    f_diff = f_total - goal_offset
    if f_diff % 2 == 1:
        return False
    f_diff //= 2
    diffs = {0}
    for f in fs:
        for d in list(diffs):
            d2 = f + d
            if d2 > f_diff:
                continue
            if d2 == f_diff:
                return True
            diffs.add(d2)
    return False


if fill(xlist, abs(gx - rx)) and fill(ylist, abs(gy - ry)):
    print("Yes")
else:
    print("No")
