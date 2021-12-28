h = 10
w = 15
x, y = 3, 5
k = 8

g = [[0 for j in range(w)] for i in range(h)]
g[x][y] = 1


def pr():
    for i in range(h):
        for j in range(w):
            print("{:5}".format(g[i][j]), end=" ")
        print()


for ite in range(k):
    g2 = [[0 for j in range(w)] for i in range(h)]
    for i in range(h):
        for j in range(w):
            for p in range(h):
                if p != i:
                    g2[i][j] += g[p][j]
            for p in range(w):
                if p != j:
                    g2[i][j] += g[i][p]
    g = g2
    print(f"### {ite}")
    pr()
