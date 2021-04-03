import random
import sys

H = 100
while True:
    w = random.randint(2, 1000)
    k = random.randint(1, w - 1)
    if (w % k) < k // 2:
        print(H, w, k)
        sys.exit(0)
