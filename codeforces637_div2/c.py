def is_ok(perm, n):
    if len(perm) != n:
        return False
    if len(set(perm)) != n:
        return False
    ppos = [0] * n
    for i, x in enumerate(perm):
        ppos[x] = i
    sim = [None] * n
    r = list(range(n))
    cnt = [1] * n
    maxcnt = 1
    nxt = None
    for i in range(n):
        chosen = ppos[i]
        if nxt is not None and chosen != nxt:
            return False
        if sim[chosen] is not None:
            return False
        sim[chosen] = i
        if chosen < n - 1 and sim[chosen + 1] is None:
            nxt = chosen + 1
        else:
            nxt = None
    return True


TC = int(input())
for _ in range(TC):
    n = int(input())
    perm = list(map(int, input().split()))
    for i in range(n):
        perm[i] -= 1
    print("Yes" if is_ok(perm, n) else "No")
