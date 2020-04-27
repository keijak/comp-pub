import collections
import sys

sys.setrecursionlimit(10 ** 9)

k, n = map(int, input().split())

hints = []
for i in range(n):
    v, w = input().split()
    hints.append((len(v), v, w))
hints.sort()


def gen_candidates(v, w, vocab, cands):
    mvocab = [None] * (k + 1)

    def gen(i, j):
        if i == len(v):
            if j == len(w):
                cands.append(mvocab.copy())
            return
        vrest = len(v) - i - 1
        vn = int(v[i])
        mv = mvocab[vn]
        for k in range(j + 1, j + 4):
            wrest = len(w) - k
            if wrest < vrest or wrest > 3 * vrest:
                continue
            word = w[j:k]
            if mv is None:
                mvocab[vn] = word
            elif mv != word:
                continue
            gen(i + 1, k)
            if mv is None:
                mvocab[vn] = None

    gen(0, 0)


def consistent(v1, v2):
    for i in range(k + 1):
        if v1[i] is None or v2[i] is None:
            continue
        if v[i] != v2[i]:
            return False
    return True


def merge_vocab(v1, v2):
    res = v1.copy()
    for i in range(k + 1):
        if v2[i] is not None:
            res[i] = v2[i]
    return res


vocab = [[None] * (k + 1)]
for (_, v, w) in hints:
    cands = []
    gen_candidates(v, w, vocab, cands)
    newvocab = []
    for v in vocab:
        for mvocab in cands:
            if consistent(v, mvocab):
                newvocab.append(merge_vocab(v, mvocab))
    vocab = newvocab

print(*vocab[0][1 : k + 1], sep="\n")
