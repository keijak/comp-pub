# K,S=open(0);K=int(K.split()[1]);u=[S[0]]
# for c in S:c==u[-1]or u.append(c)
# print(len(S)-max(len(u)-2*K,1))


n,s=open(0);n,k=map(int,n.split());print(n-max(sum(i==j for i,j in zip(s,s[1:]))-2*k,1))



print(min(n-1,k*2+))
print(min(n-1,k*2+sum(i==j for i,j in zip(s,s[1:]))))



n,s=open(0);n,k=map(int,n.split());


  min(n-1,k*2+sum(i==j for i,j in zip(s,s[1:]))))


N,K=map(int,input().split());S=input();u=[S[0]]
for c in S:c==u[-1]or u.append(c)
print(len(S)-max(len(u)-2*K,1))
