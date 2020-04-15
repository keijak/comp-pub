N, H = map(int, input().split())
sword = [tuple(map(int, input().split()))
         for _ in range(N)]
sword.sort(key=lambda x: (x[1], x[0]), reverse=True)

base = 0
a0, b0 = sword[base]
for i in range(1, N):
    ai, bi = sword[i]
    if ai > a0 or (ai == a0 and bi < b0):
        base = i
        a0, b0 = ai, bi


def solve():
    cnt = 0
    h = H
    bi = 1 if base == 0 else 0
    while h > 0:
        if h < b0:
            cnt += 1
            break
        if bi >= len(sword) or a0 >= sword[bi][1]:
            if a0 < b0:
                h -= b0
                cnt += 1
            if h % a0 == 0:
                cnt += h // a0
                break
            else:
                cnt += h // a0 + 1
                break
        h -= sword[bi][1]
        bi += 1
        if bi == base:
            bi += 1
        cnt += 1
    return cnt


print(solve())
