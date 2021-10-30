def main():
    fl = list(reversed('B1,B2,B3,B4,B5,B6,B7,B8,B9'.split(','))) + '1F,2F,3F,4F,5F,6F,7F,8F,9F'.split(',')

    s, t = input().split()

    si = fl.index(s)
    ti = fl.index(t)

    print(abs(si - ti))


main()
