n,*a=map(int,open(0).read().split());c=0;r=1;m=a[0]
for i in range(n):
 while r<n and not(a[r]&m):m|=a[r];r+=1
 c+=r-i;m&=~a[i]
print(c)
