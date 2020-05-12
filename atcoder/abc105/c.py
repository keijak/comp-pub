N = int(input())
ans = [0]


def solve(n):
    if n == 0:
        return
    if n > 0:
        p, a, k = 1, 1, 0
    else:
        p, a, k = -2, -2, 1
    while abs(n) > abs(a):
        p *= 4
        a += p
        k += 2
    while len(ans) <= k:
        ans.append(0)
    ans[k] = 1
    solve(n - p)


solve(N)
print("".join([str(x) for x in reversed(ans)]))
