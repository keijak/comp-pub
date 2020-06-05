import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()


def main():
    N, M = inm()
    adj = [[] for _ in range(N)]
    for i in range(M):
        a, b = inm()
        a -= 1
        b -= 1
        assert a != b
        adj[a].append(b)
        adj[b].append(a)
    topic = inl()
    assert len(topic) == N
    cntr = 0
    order = [None] * N

    def solve(cur):
        nonlocal cntr
        t = topic[cur]
        covered = set()
        for nei in adj[cur]:
            if topic[nei] == t:
                return False
            if topic[nei] < t:
                covered.add(topic[nei])
        if len(covered) < t - 1:
            # print("cannot place: {} topic  covered {}".format(cur, t, covered))
            return False
        if order[cur] is None:
            order[cur] = cntr
            cntr += 1
        return True

    pres = sorted([(topic[i], i) for i in range(N)])
    for i in range(N):
        _, j = pres[i]
        if order[j] is None:
            if not solve(j):
                print(-1)
                return
    ans = [None] * N
    for i in range(N):
        ans[order[i]] = i + 1
    print(*ans)


main()
