from collections import Counter

_ = input()
c = Counter(input())
print(c["R"] % 2 + c["G"] % 2 + c["B"] % 2)
