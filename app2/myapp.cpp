#include<tmwp>
#include<stdio.h>
#include<fstream>
#include<string.h>
using namespace tmwp;
using namespace std;
int parseInt(const char *p)
{
int value=p[0]-48; 
for(int i=1;p[i]!='\0';i++)
{
value=value*10+(p[i]-48);
}
return value;
}

void getMarks(Request &request,Response &response)
{
int rollNumber=parseInt(request.get("rl").c_str());
cout<<endl<<"rl is "<<rollNumber<<endl;
if(rollNumber==101)
{
request.setInt("physics",90);
request.setInt("chemistry",85);
request.setInt("maths",92);
request.setInt("english",86);
request.setInt("hindi",80);
}else if(rollNumber==102)
{
request.setInt("physics",70);
request.setInt("chemistry",75);
request.setInt("maths",72);
request.setInt("english",76);
request.setInt("hindi",70);
}else if(rollNumber==103)
{
request.setInt("physics",88);
request.setInt("chemistry",72);
request.setInt("maths",75);
request.setInt("english",79);
request.setInt("hindi",82);
}else if(rollNumber==104)
{
request.setInt("physics",58);
request.setInt("chemistry",68);
request.setInt("maths",62);
request.setInt("english",70);
request.setInt("hindi",75);
}else if(rollNumber==105)
{
request.setInt("physics",54);
request.setInt("chemistry",68);
request.setInt("maths",52);
request.setInt("english",50);
request.setInt("hindi",60);
}
request.forward("marks.sct");
}
int main()
{
TMWebProjector server(8080);
server.onRequest("getMarks",getMarks);
server.start();
return 0;
}