import sys

n, k = map(int, input().split())
bins = []
for _ in range(n):
    bins.append(input())

segs = [
    "1110111",
    "0010010",
    "1011101",
    "1011011",
    "0111010",
    "1101011",
    "1101111",
    "1010010",
    "1111111",
    "1111011",
]


def diff(s1, s2):
    cnt = 0
    for i in range(7):
        if s1[i] == s2[i]:
            continue
        if s1[i] == "1" and s2[i] == "0":
            cnt += 1
        else:
            return 3000
    return cnt


minrequired = [0] * n
for i in range(n):
    minrequired[i] = min(diff(segs[j], bins[i]) for j in range(10))
acum = [0] * n
for i in range(n - 2, -1, -1):
    acum[i] = acum[i + 1] + minrequired[i + 1]

stack = [(0, k, 0)]
while stack:
    i, sticks, ans = stack.pop()
    if i == n:
        if sticks == 0:
            print(ans)
            sys.exit()
        continue
    digit = bins[i]
    for j in range(10):
        sd = diff(segs[j], digit)
        if sticks - sd < acum[i]:
            continue
        stack.append((i + 1, sticks - sd, 10 * ans + j))


print(-1)
