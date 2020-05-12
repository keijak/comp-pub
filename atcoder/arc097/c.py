s = input()
k = int(input())


def ins(smallests, x):
    if len(smallests) == k and x > smallests[-1]:
        return
    if x in smallests:
        return
    smallests.append(x)
    smallests.sort()
    while len(smallests) > k:
        smallests.pop()


def solve():
    smallests = []
    for i in range(len(s)):
        for j in range(i + 1, min(i + k + 1, len(s))):
            ins(smallests, s[i:j])
    return smallests[-1]


print(solve())
