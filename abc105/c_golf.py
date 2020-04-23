n=int(input());r=0
while n:
 p,a,k=[(1,1,1),(-2,-2,2)][n<0]
 while abs(n)>abs(a):p*=4;a+=p;k<<=2
 r|=k;n-=p
print(bin(r)[2:])
