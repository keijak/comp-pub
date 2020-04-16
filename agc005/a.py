s = 0
for x in input():
    if x == 'S':
        s += 1
    elif s > 0:
        s -= 1
print(2*s)