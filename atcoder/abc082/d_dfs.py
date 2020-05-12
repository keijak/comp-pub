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
while head < len(s):
    tcnt = 0
    while head < len(s) and s[head] == "T":
        tcnt += 1
        head += 1
    fcnt = 0
    while head < len(s) and s[head] == "F":
        fcnt += 1
        head += 1
    inst.append((tcnt % 2, fcnt))

f_total = sum([f for (_, f) in inst])
f_rem = [0] * (len(inst) + 1)
f_rem[len(inst)] = 0
for i in range(len(inst) - 1, -1, -1):
    f_rem[i] = f_rem[i + 1] + inst[i][1]
assert f_rem[0] == f_total


visited = set()


def reach(i, x, y, dx, dy):
    dist = abs(gx - x) + abs(gy - y)
    # print(f"i={i} ({x},{y}) dist={dist}: inst={inst[i:]}")
    if i == len(inst):
        return dist == 0
    if (i, x, y, dx, dy) in visited:
        return False
    visited.add((i, x, y, dx, dy))
    if dist > f_rem[i] or dist % 2 != f_rem[i] % 2:
        return False
    t, f = inst[i]
    if t:
        dy, dx = dx, -dy
    if reach(i + 1, x + f * dx, y + f * dy, dx, dy):
        return True
    if reach(i + 1, x - f * dx, y - f * dy, -dx, -dy):
        return True
    return False


print("Yes" if reach(0, rx, ry, 1, 0) else "No")
