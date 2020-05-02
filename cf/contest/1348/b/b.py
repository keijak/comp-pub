import sys

sys.setrecursionlimit(10 ** 8)
read = sys.stdin.buffer.read
readline = sys.stdin.buffer.readline
readlines = sys.stdin.buffer.readlines


def solve(n, k, a):
    nextid = 1
    seen = set()
    uniq = []
    for x in a:
        if x not in seen:
            uniq.append(x)
            seen.add(x)
    if len(seen) > k:
        print(-1)
        return
    while len(uniq) < k:
        while nextid in seen:
            nextid += 1
        uniq.append(nextid)
        seen.add(nextid)
    j = 0
    b = []
    while j < n:
        for x in uniq:
            b.append(x)
            if x == a[j]:
                j += 1
                if j == n:
                    break
    print(len(b))
    print(*b)


def main():
    T = int(readline())
    for i in range(T):
        n, k = map(int, readline().split())
        assert 1 <= k <= n <= 100
        a = list(map(int, readline().split()))
        assert len(a) == n
        solve(n, k, a)


if __name__ == "__main__":
    main()
