#include<iostream>
using namespace std;
string toString(int integer)
{
char value[25];
int num=integer;
int dc=0;
while(num>0)
{
num=num/10;
dc++;
}
value[dc]='\0';
dc--;
num=integer;
while(num>0)
{
value[dc]=(num%10)+48;
num=num/10;
dc--;
}
cout<<"value=("<<value<<")"<<endl;
string s=value;
return s;
}

int main()
{
int i;
cout<<"enter a number";
cin>>i;
string s=toString(i);
cout<<s;
return 0;
}