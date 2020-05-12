def g(x):
 p=1
 while p*2<=x:p*=2
 return[0,1,3,0][x]if x<4else(p*((x-p+1)&1))|g(x-p)
a,b=map(int,input().split());print(g(b)^g(a-1))