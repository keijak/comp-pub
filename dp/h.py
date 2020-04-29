import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

H, W = map(int, readline().split())
grid = []
for i in range(H):
    grid.append(readline().decode("utf-8").rstrip())


def solve():
    print(grid)


if __name__ == "__main__":
    solve()
