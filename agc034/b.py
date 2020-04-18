import sys
s = sys.stdin.readline().rstrip()
N = len(s)

def next_abc(ac, start):
    for i in range(start, N):
        if ac and s.startswith('BC', i):
            return ac, i+2
        elif s[i] == 'A':
            ac += 1
        else:
            ac = 0
    return 0, N

ans = ac = i = 0
while i < N:
    ac, i = next_abc(ac, i)
    ans += ac
print(ans)
