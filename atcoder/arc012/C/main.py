import sys

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
# debug = lambda *a, **kw: print(*a, **kw, file=sys.stderr)

L = 19
board = [None] * L
for i in range(L):
    row = ins()
    assert len(row) == L
    board[i] = row


def possible():
    bcnt = wcnt = 0
    bhwin = whwin = None

    def hcheck():
        nonlocal bcnt, wcnt, bhwin, whwin
        for r in range(L):
            bstreak = []
            wstreak = []
            for c in range(L):
                cell = board[r][c]
                if cell == "o":
                    bcnt += 1
                    wstreak = []
                    bstreak.append((r, c))
                    if len(bstreak) == 5:
                        if bhwin is not None:
                            return False
                        bhwin = bstreak
                    elif len(bstreak) == 10:
                        return False
                elif cell == "x":
                    wcnt += 1
                    bstreak = []
                    wstreak.append((r, c))
                    if len(wstreak) == 5:
                        if whwin is not None:
                            return False
                        whwin = wstreak
                    elif len(wstreak) == 10:
                        return False
                else:
                    assert cell == "."
                    bstreak = []
                    wstreak = []
        return True

    bvwin = wvwin = None

    def vcheck():
        nonlocal bvwin, wvwin
        for c in range(L):
            bstreak = []
            wstreak = []
            for r in range(L):
                cell = board[r][c]
                if cell == "o":
                    wstreak = []
                    bstreak.append((r, c))
                    if len(bstreak) == 5:
                        if bvwin is not None:
                            return False
                        bvwin = bstreak
                    elif len(bstreak) == 10:
                        return False
                elif cell == "x":
                    bstreak = []
                    wstreak.append((r, c))
                    if len(wstreak) == 5:
                        if wvwin is not None:
                            return False
                        wvwin = wstreak
                    elif len(wstreak) == 10:
                        return False
                else:
                    assert cell == "."
                    bstreak = []
                    wstreak = []
        return True

    bd1win = wd1win = None

    def d1check():
        nonlocal bd1win, wd1win
        for c in range(2 * L):
            bstreak = []
            wstreak = []
            for r in range(L):
                if c - r >= L:
                    continue
                if c - r < 0:
                    break
                cell = board[r][c - r]
                if cell == "o":
                    wstreak = []
                    bstreak.append((r, c - r))
                    if len(bstreak) == 5:
                        if bd1win is not None:
                            return False
                        bd1win = bstreak
                    elif len(bstreak) == 10:
                        return False
                elif cell == "x":
                    bstreak = []
                    wstreak.append((r, c - r))
                    if len(wstreak) == 5:
                        if wd1win is not None:
                            return False
                        wd1win = wstreak
                    elif len(wstreak) == 10:
                        return False
                else:
                    assert cell == "."
                    bstreak = []
                    wstreak = []
        return True

    bd2win = wd2win = None

    def d2check():
        nonlocal bd2win, wd2win
        for c in range(L - 1, -L - 1, -1):
            bstreak = []
            wstreak = []
            for r in range(L):
                if r + c < 0:
                    continue
                if r + c >= L:
                    break
                cell = board[r][r + c]
                if cell == "o":
                    wstreak = []
                    bstreak.append((r, r + c))
                    if len(bstreak) == 5:
                        if bd2win is not None:
                            return False
                        bd2win = bstreak
                    elif len(bstreak) == 10:
                        return False
                elif cell == "x":
                    bstreak = []
                    wstreak.append((r, r + c))
                    if len(wstreak) == 5:
                        if wd2win is not None:
                            return False
                        wd2win = wstreak
                    elif len(wstreak) == 10:
                        return False
                else:
                    assert cell == "."
                    bstreak = []
                    wstreak = []
        return True

    if not hcheck():
        return False
    if not vcheck():
        return False
    if not d1check():
        return False
    if not d2check():
        return False

    if bcnt != wcnt and bcnt != wcnt + 1:
        return False

    bwin = bhwin or bvwin or bd1win or bd2win
    wwin = whwin or wvwin or wd1win or wd2win
    if bwin and wwin:
        return False

    def wincheck(*wins):
        wins = [w for w in wins if w]
        if not wins:
            return False
        if len(wins) == 1:
            return True
        s = set(wins[0])
        for t in wins[1:]:
            s &= set(t)
        if len(s) != 1:
            return False
        cross = list(s)[0]
        for w in wins:
            assert 5 <= len(w) < 10
            i = w.index(cross)
            if i >= 5 or len(w) - 1 - i >= 5:
                return False
        return True

    if bwin:
        if bcnt != wcnt + 1:
            return False
        if not wincheck(bhwin, bvwin, bd1win, bd2win):
            return False
    if wwin:
        if bcnt != wcnt:
            return False
        if not wincheck(whwin, wvwin, wd1win, wd2win):
            return False

    return True


print("YES" if possible() else "NO")
