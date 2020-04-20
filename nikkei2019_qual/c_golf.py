print(sum(p[1+i%2]for i,p in enumerate(sorted([(lambda a,b:(-a-b,a,-b))(*map(int,input().split()))for i in range(int(input()))]))))
