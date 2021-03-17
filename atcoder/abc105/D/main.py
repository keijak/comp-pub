import sys
import collections

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines

N, M = map(int, readline().split())
A = list(map(int, readline().split()))


def solve():
    acum = [0] * (N + 1)
    acum[0] = 0
    c = collections.Counter()
    c[0] = 1

    ans = 0
    for i in range(N):
        x = (acum[i] + A[i]) % M
        ans += c[x]
        c[x] += 1
        acum[i + 1] = x
    return ans


if __name__ == "__main__":
    print(solve())
