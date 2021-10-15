import sys

n = int(input())
n2 = n * 2


def query(a):
    assert len(a) == n
    print('?', *a, flush=True)
    res = input()
    if res == 'Red':
        return True
    elif res == 'Blue':
        return False
    else:
        exit(-1)


ans = [None] * (n2 + 1)


def answer():
    print('!', *ans[1:], flush=True)
    exit(0)


ini = query(list(range(1, n + 1)))
r = n + 1
while r <= n2:
    res = query(list(range(r - n + 1, r + 1)))
    if res == ini:
        r += 1
        continue
    ans[r] = res
    ans[r - n] = ini
    print('ok:', ans, file=sys.stderr)
    for i in range(1, n2+1):
        
l = r - n  # ini color
