N = int(input())

ss = ['a']
for i in range(1, N):
    tmp = []
    for s in ss:
        newc = ord(max(s)) + 1
        for j in range(ord('a'), newc+1):
            tmp.append(s + chr(j))
    ss = tmp

for s in sorted(ss):
    print(s)
