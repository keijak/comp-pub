import datetime

y = int(input())
m = int(input())
d = int(input())

d1 = datetime.date(y, m, d)
d2 = datetime.date(2014, 5, 17)
print((d2 - d1).days)
