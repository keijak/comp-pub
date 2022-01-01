#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using V=vector<atcoder::modint998244353>;
int main(){
 size_t N,Q;cin>>N>>Q;deque<V>q;
 for(long long i=0,a;i<N;++i)cin>>a,q.push_back(V{a-1,1});
 while(q.size()>1)q.push_back(atcoder::convolution(q[0],q[1])),q.back().resize(min(size(q.back()),N+1)),q.pop_front(),q.pop_front();
 for(int i=0,b;i<Q;++i)cin>>b,cout<<q.front()[b].val()<<'\n';
}