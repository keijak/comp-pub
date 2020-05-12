N, P = map(int, input().split())
snacks = []
for i in range(N):
    price, score = map(int, input().split())
    snacks.append((price, score))
snacks.sort(reverse=True)

EX = 110
score_t = [None] * (P + EX)
path_t = [None] * (P + EX)
score_t[0] = 0
for i in range(N):
    price, score = snacks[i]
    for p, mscore in enumerate(list(score_t)):
        if mscore is None:
            continue
        if p > P or p + price >= P + EX:
            break
        maxscore = mscore + score
        if score_t[p + price] is None or score_t[p + price] < maxscore:
            score_t[p + price] = maxscore
            path_t[p + price] = p


def path(p):
    res = []
    while path_t[p] is not None:
        res.append(p - path_t[p])
        p = path_t[p]
    return res


maxscore = -1
for p, s in enumerate(score_t):
    if s is None:
        continue
    # print(f"> {s} at price {p}: path: {path(p)}")
    maxscore = max(maxscore, s)
print(maxscore)
