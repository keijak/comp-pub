#include<stdio.h>
int H,W,K,D[10][10][3]={{{1,0,0},{1,1,0},{1,2,2},{1,4,9},{1,6,30},{1,12,91},{1,18,258},{1,34,729},{1,58,2018},{1,106,5613}},{{1,1,0},{1,5,9},{1,12,91},{1,38,738},{1,106,5613}},{{1,2,2},{1,12,91},{1,62,2022}},{{1,4,9},{1,38,738}},{{1,6,30},{1,106,5613}},{{1,12,91}},{{1,18,258}},{{1,34,729}},{{1,58,2018}},{{1,106,5613}}};main(){scanf("%d%d%d",&H,&W,&K);printf("%d\n",D[--H][--W][--K]);}
