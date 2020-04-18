from bisect import*
(N,),A,B,C=[sorted(map(int,input().split()))for _ in[0]*4]
print(sum(bisect_left(A,b)*(N-bisect(C,b))for b in B))
