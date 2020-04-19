N, K = map(int, input().split())
S = input()
assert len(S) == N


def count_unhappy():
    unhappy = sum([x != y for x, y in zip(S, S[1:])])
    return max(unhappy - 2 * K, 1)


print(N - count_unhappy())
