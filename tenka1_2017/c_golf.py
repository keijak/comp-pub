N=int(input())
for h in range(3501):
 for n in range(h):
  d=4*h*n-N*(h+n)
  if d>0 and N*h*n%d<1:print(h,n,N*h*n//d);exit()