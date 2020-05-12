N = int(input())

def check(k):
    h3 = h5 = h7 = False
    while k:
        d = k%10
        h3 |= d == 3
        h5 |= d == 5
        h7 |= d == 7
        k //= 10
    return h3 and h5 and h7

ans = 0
stack = [3,5,7]
while stack:
    k = stack.pop()
    if k > N:
        continue
    if check(k):
        ans += 1
    stack.extend([k*10+3, k*10+5, k*10+7])

print(ans)