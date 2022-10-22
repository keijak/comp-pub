from decimal import Decimal

a, b = map(int, input().split())

if a == 0:
    print("0.000")
else:
    d = Decimal(b) / a * 1000
    x = round(d) / 1000
    print("%0.3f" % x)
