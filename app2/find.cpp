#include<stdio.h>
#include<iostream>
#include<string.h>
using namespace std;
int main()
{
char findx[5];
findx[0]='$';
findx[1]='{';
findx[2]='x';
findx[3]='}';
findx[4]='\0';
char findy[5];
findy[0]='$';
findy[1]='{';
findy[2]='y';
findy[3]='}';
findy[4]='\0';
char find[5];
char stringOfX[15];
char stringOfY[15];
char c;
int z,y;
int x=0;
cout<<"X=";
cin>>z;
cout<<"Y=";
cin>>y;
sprintf(stringOfX,"%d",z);
sprintf(stringOfY,"%d",y);
FILE *f;
f=fopen("jayesh.xyz","r");
FILE *t;
t=fopen("tmp.tmp","a");
while(1)
{
c=fgetc(f);
if(feof(f))break;
if(c=='$')
{
find[0]=c;
for(x=1;x<=3;x++)
{
c=fgetc(f);
find[x]=c;
}
find[x]='\0';
if(strcmp(find,findx)==0)
{
for(x=0;stringOfX[x]!='\0';x++)
{
fputc(stringOfX[x],t);
}
}else if(strcmp(find,findy)==0)
{
for(x=0;stringOfY[x]!='\0';x++)
{
fputc(stringOfY[x],t);
}
}else
{
for(x=0;x<=3;x++)
{
fputc(find[x],t);
}
}
}
else
{
cout<<"("<<c<<")";
fputc(c,t);
}
}
fclose(f);
fclose(t);
remove("jayesh.xyz");
rename("tmp.tmp","jayesh.xyz");
remove("tmp.tmp");

return 0;
}