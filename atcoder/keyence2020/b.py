N = int(input())
xl = []
for i in range(N):
    x, l = map(int, input().split())
    xl.append((x, l))
xl.sort()

kept = []
cur = 0
for i in range(1, N):
    x_cur, l_cur = xl[cur]
    x_next, l_next = xl[i]
    if x_cur + l_cur <= x_next - l_next:
        kept.append(cur)
        cur = i
    elif x_cur + l_cur > x_next + l_next:
        cur = i
kept.append(cur)

print(len(kept))
