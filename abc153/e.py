H, N = map(int,input().split())
magic = []
for i in range(N):
    a,b = map(int,input().split())
    magic.append((a, b))
magic.sort(reverse=True)
amax, _ = magic[0]

INF = 1<<60

def solve():
    btab = [INF] * (H+1)
    btab[0] = 0
    oneshot = min(amax, H)
    for h in range(1, oneshot+1):
        btab[h] = min([b for a, b in magic if a >= h])
    for h in range(2, H+1):
        for a, b in magic:
            c = b + btab[max(h-a, 0)]
            if c < btab[h]:
                btab[h] = c
    return btab[H]

print(solve())