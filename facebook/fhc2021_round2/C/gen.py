import random

print(1)

R = random.randint(10, 30)
C = random.randint(10, 30)
K = random.randint(1, R)

print(R, C, K)
for i in range(R):
    for j in range(C):
        x = random.randint(0, 1)
        print("X" if x else ".", end="")
    print()
