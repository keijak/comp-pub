import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, K = map(int, input().split())
coords = []
for i in range(N):
    x, y = map(int, input().split())
    coords.append((x, y))
coords.sort()


def solve():
    minarea = 1 << 62
    for t in range(N):
        _, yt = coords[t]
        for b in range(N):
            _, yb = coords[b]
            if yb < yt:
                continue
            h = yb - yt
            xs = []
            for i in range(N):
                xi, yi = coords[i]
                if yi < yt or yi > yb:
                    continue
                xs.append(xi)
            for i in range(K - 1, len(xs)):
                w = xs[i] - xs[i - K + 1]
                a = w * h
                minarea = min(minarea, a)
    return minarea


if __name__ == "__main__":
    print(solve())
