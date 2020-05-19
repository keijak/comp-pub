import sys

sys.setrecursionlimit(10 ** 8)
ni = lambda: int(sys.stdin.readline())
nm = lambda: map(int, sys.stdin.readline().split())
nl = lambda: list(nm())
ns = lambda: sys.stdin.readline().rstrip()

R, G, B, N = nm()


def solve():
    ans = 0
    for r in range(N // R + 2):
        for g in range(N // G + 2):
            k = N - (r * R + g * G)
            if k < 0:
                break
            if k % B == 0:
                ans += 1
    return ans


if __name__ == "__main__":
    print(solve())
