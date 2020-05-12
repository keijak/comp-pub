h,w,a,b=map(int,input().split())
for i in range(h):print('01'[i<b]*a+'01'[i>=b]*(w-a))