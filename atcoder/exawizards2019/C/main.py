import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

N, Q = nm()
board = ns()
assert len(board) == N
spells = []
for i in range(Q):
    t, d = ns().split()
    spells.append((t, d))


def solve():
    head, tail = -1, N
    for i in range(Q - 1, -1, -1):
        t, d = spells[i]
        if d == "L":
            if head + 1 < N and t == board[head + 1]:
                head += 1
            if tail < N and t == board[tail]:
                tail += 1
        if d == "R":
            if tail - 1 >= 0 and t == board[tail - 1]:
                tail -= 1
            if head >= 0 and t == board[head]:
                head -= 1
    return N - (head + 1) - (N - tail)


print(solve())
