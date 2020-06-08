aI, aO, _, aJ, aL, _, _ = map(int, input().split())


def f(i, j, l):
    return aO + 2 * (i // 2 + j // 2 + l // 2)


ans = f(aI, aJ, aL)
if aI and aJ and aL:
    ans = max(ans, 3 + f(aI - 1, aJ - 1, aL - 1))
print(ans)
