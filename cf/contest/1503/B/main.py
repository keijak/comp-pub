from sys import stdout

n = int(input())

ne = (1, 1)
no = (1, 2)


def move_ne(b):
    global ne
    i, j = ne
    print(b, i, j)
    stdout.flush()
    if i <= n and j + 2 <= n:
        ne = (i, j + 2)
    elif i + 1 <= n:
        if (i + 1) % 2 == 1:
            ne = (i + 1, 1)
        else:
            ne = (i + 1, 2)
    else:
        ne = None


def move_no(b):
    global no
    i, j = no
    print(b, i, j)
    stdout.flush()
    if i <= n and j + 2 <= n:
        no = (i, j + 2)
    elif i + 1 <= n:
        if (i + 1) % 2 == 0:
            no = (i + 1, 1)
        else:
            no = (i + 1, 2)
    else:
        no = None


for i in range(n ** 2):
    a = int(input())
    if a == 1:
        if ne is None:
            move_no(3)
        else:
            move_ne(2)
    else:
        if no is None:
            move_ne(3 if a == 2 else 2)
        else:
            move_no(1)
