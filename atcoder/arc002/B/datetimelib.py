import datetime


def solve():
    y, m, d = map(int, input().split("/"))
    dt = datetime.date(y, m, d)
    while True:
        if dt.year % (dt.month * dt.day) == 0:
            print("{:04}/{:02}/{:02}".format(dt.year, dt.month, dt.day))
            return
        dt += datetime.timedelta(days=1)


if __name__ == "__main__":
    solve()
