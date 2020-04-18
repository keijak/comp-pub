N,K = map(int, input().split())
D = list(map(int, input().split()))
dt = [True] * 10
for d in D:
    dt[d] = False

def check(x):
    while x:
        if not dt[x%10]:
            return False
        x //= 10
    return True

for i in range(N, 10*N):
    if check(i):
        print(i)
        break
