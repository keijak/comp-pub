r1, c1 = map(int, input().split())
r2, c2 = map(int, input().split())


# Is (r2, c2) reachable from (r, c) in one step?
def one_step(r, c):
    if r + c == r2 + c2:
        return True
    if r - c == r2 - c2:
        return True
    return abs(r - r2) + abs(c - c2) <= 3


def solve():
    if (r1, c1) == (r2, c2):
        return 0
    if one_step(r1, c1):
        return 1
    if (r1 + c1) % 2 == (r2 + c2) % 2:
        return 2
    for dr in range(-3, 4):
        for dc in range(-3, 4):
            if abs(dr) + abs(dc) > 3:
                continue
            if one_step(r1 + dr, c1 + dc):
                return 2
    return 3


print(solve())
