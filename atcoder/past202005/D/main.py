import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
dbg = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

N = ini()
board = [None] * 5
for i in range(5):
    board[i] = ins()
    assert len(board[i]) == 4 * N + 1

dmap = {
    0: [".###", ".#.#", ".#.#", ".#.#", ".###"],
    1: ["..#.", ".##.", "..#.", "..#.", ".###"],
    2: [".###", "...#", ".###", ".#..", ".###"],
    3: [".###", "...#", ".###", "...#", ".###"],
    4: [".#.#", ".#.#", ".###", "...#", "...#"],
    5: [".###", ".#..", ".###", "...#", ".###"],
    6: [".###", ".#..", ".###", ".#.#", ".###"],
    7: [".###", "...#", "...#", "...#", "...#"],
    8: [".###", ".#.#", ".###", ".#.#", ".###"],
    9: [".###", ".#.#", ".###", "...#", ".###"],
}


def check(st, lamps):
    for i in range(5):
        if board[i][st : st + 4] != lamps[i]:
            return False
    return True


def solve():
    i = 0
    ans = []
    while i < 4 * N:
        digit = None
        for d in range(10):
            if check(i, dmap[d]):
                digit = d
                break
        assert digit is not None
        ans.append(str(d))
        i += 4
    return "".join(ans)


print(solve())
