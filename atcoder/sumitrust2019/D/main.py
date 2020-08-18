N = int(input())
S = list(map(int,input()))
assert len(S) == N

def solve():
    t1, t2, t3 = set(), set(), set()
    for i in range(N):
        z = S[i]
        for x, y in t2:
            t3.add((x, y, z))
        for x in t1:
            t2.add((x, z))
        t1.add(z)
    return len(t3)

if __name__ == "__main__":
    print(solve())
