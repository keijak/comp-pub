from collections import defaultdict
H, W, K = map(int, input().split())
cake = []
sbdict = defaultdict(list)
sbrows = set()

for i in range(H):
    line = input()
    assert len(line) == W
    cake.append(line)
    for j in range(W):
        if line[j] == '#':
            sbrows.add(i)
            sbdict[i].append(j)
sbrows = sorted(sbrows)

def divide():
    dcake = [[None]*W for _ in range(H)]
    color = 1
    for r in sbrows:
        cs = list(sorted(sbdict[r]))
        cs.append(W)  # sentinel
        prev = 0
        for i in range(len(cs)-1):
            for j in range(prev, cs[i+1]):
                dcake[r][j] = color
            color += 1
            prev = cs[i+1]
    # Fill blank lines.
    prev = 0
    for i in range(len(sbrows)):
        for r in range(prev, sbrows[i]):
            dcake[r] = list(dcake[sbrows[i]])
        prev = sbrows[i] + 1
    lastr = sbrows[-1]
    for r in range(lastr+1, H):
        dcake[r] = list(dcake[lastr])
    return dcake

dcake = divide()
for r in range(H):
    print(' '.join('{:4}'.format(x) for x in dcake[r]))
