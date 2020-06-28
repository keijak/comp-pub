import sys
import numpy as np
import numba

sys.setrecursionlimit(10 ** 8)
ini = lambda: int(sys.stdin.readline())
inm = lambda: map(int, sys.stdin.readline().split())
inl = lambda: list(inm())
ins = lambda: sys.stdin.readline().rstrip()
debug = lambda *a, **kw: print("\033[33m", *a, "\033[0m", **dict(file=sys.stderr, **kw))


@numba.njit("(i8, i2[:], i4[:,:])", cache=True)
def optimize(D, C, S):
    NUM_ITERS = 30_000_000

    def calc_score(sol):
        last = -np.ones((26, D + 1))
        score = 0
        for d in range(D):
            ct = sol[d]
            score += S[d, ct]
            for i in range(26):
                last[i, d + 1] = last[i, d]
            last[ct, d + 1] = d
            for i in range(26):
                score -= C[i] * (d - last[i, d + 1])
        return score, last

    sol = np.zeros(D, dtype=np.int16)
    last_tmp = -np.ones(26, dtype=np.int16)
    for d in range(D):
        score = 0
        mi = -1
        for i in range(26):
            s = S[d, i] - C[i] * (d - last_tmp[i])
            if score < s:
                score = s
                mi = i
        sol[d] = mi
        last_tmp[mi] = d

    sol_bak = sol.copy()
    max_score, last = calc_score(sol)
    last_bak = last.copy()

    def update(d, before, after):
        score = max_score
        score -= S[d, before]
        score += S[d, after]
        lb = last[before, d]
        la = last[after, d]
        da = db = 0
        for x in range(d, D):
            if x > d and sol[x] == before:
                break
            last[before, x + 1] = lb
            db += 1
        score -= C[before] * (d - lb) * db
        for x in range(d, D):
            if x > d and sol[x] == after:
                break
            last[after, x + 1] = d
            da += 1
        score += C[after] * (d - la) * da
        sol[d] = after
        return score

    tried = numba.typed.Dict.empty(
        key_type=numba.types.int32, value_type=numba.types.boolean
    )
    for i in range(NUM_ITERS):
        new_d = np.random.randint(0, D)
        c_before = sol[new_d]
        c_after = np.random.randint(0, 26)
        if c_before == c_after:
            continue
        candidate = new_d * 26 + c_after
        if candidate in tried:
            continue
        tried[candidate] = True
        new_score = update(new_d, c_before, c_after)
        if max_score < new_score:
            max_score = new_score
            sol_bak[:] = sol
            last_bak[c_before, :] = last[c_before, :]
            last_bak[c_after, :] = last[c_after, :]
            tried.clear()
        else:
            sol[:] = sol_bak
            last[c_before, :] = last_bak[c_before, :]
            last[c_after, :] = last_bak[c_after, :]
    return sol


def main():
    D = ini()
    C = np.array(inl(), dtype="int16")
    assert len(C) == 26
    S = np.zeros((D, 26), dtype="int32")
    for i in range(D):
        S[i, :] = inl()

    sol = optimize(D, C, S)
    print(*(sol + 1), sep="\n")


main()
