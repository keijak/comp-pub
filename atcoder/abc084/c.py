N = int(input())
data = []
for i in range(N-1):
    c, s, f = map(int,input().split())
    data.append((c, s, f))

for i in range(N-1):
    duration, start, freq = data[i]
    total = start + duration
    for j in range(i+1, N-1):
        d, s, f = data[j]
        if total < s:
            total = s + d
        elif total % f == 0:
            total += d
        else:
            total += f - (total % f) + d
    print(total)
print(0)
