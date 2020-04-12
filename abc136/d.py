s = input()
n = len(s)
tol = [0] * n
tor = [0] * n

def futurepos(i):
    if s[i] == 'R':
        t = tol[i]
        lpos = i + t
        if t%2 == 0:
            return lpos
        else:
            return lpos-1
    else:
        t = tor[i]
        rpos = i - t
        if t%2 == 0:
            return rpos
        else:
            return rpos+1


def main():
    for i in range(n):
        if s[i] == 'R':
            tor[i] = 0
        else:
            tor[i] = tor[i-1] + 1
        j = n-1-i
        if s[j] == 'L':
            tol[j] = 0
        else:
            tol[j] = tol[j+1] + 1

    pos = [0] * n
    for i in range(n):
        k = futurepos(i)
        pos[k] += 1
    print(*pos, sep=' ')

main()