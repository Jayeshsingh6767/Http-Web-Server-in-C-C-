#include<tmwp>
#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<map>
using namespace std;
using namespace tmwp;
Response::Response(int clientSocketDescriptor)
{
this->clientSocketDescriptor=clientSocketDescriptor;
isClosed=false;
this->headerCreated=false;
}
void Response::createHeader()
{
char header[51];
strcpy(header,"HTTP/1.1 200 OK\nContent-Type:text/html\n\n");
send(clientSocketDescriptor,header,strlen(header),0);
this->headerCreated=true;
}
void Response::write(char *response)
{
if(response==NULL) return;
int len=strlen(response);
if(len==0)return;
if(!this->headerCreated) createHeader();
send(clientSocketDescriptor,response,len,0);
}
void Response::close()
{
if(isClosed)return;
closesocket(this->clientSocketDescriptor);
this->isClosed=true;
}
 

void Request::setInt(string key,int data)
{
intDataMap.insert(pair<string,int>(key,data));
}
int Request::getInt(string key)
{
map<string,int>::iterator i;
i=intDataMap.find(key);
if(i==intDataMap.end())return 0;
return i->second;
}
void Request::forward(char *request)
{
if(request==NULL) return;
int length=strlen(request);
forwardedRequest=(char *)malloc(sizeof(char)*length);
strcpy(forwardedRequest,request);
isRequestForwarded=1;
}
int Request::tellIsRequestForwarded()
{
return this->isRequestForwarded;
}
char * Request::getForwardedRequest()
{
return this->forwardedRequest;
}
string Request::get(string key)
{
map<string,string>::iterator i;
i=dataMap.find(key);
if(i==dataMap.end())return "";
return i->second;
}
/*typedef struct _request
{
char *method;
char *resource;
char isClientSideTechnologyResource;
char *mimeType;
int dataCount;
char ** data;
}REQUEST;*/
int extentionEquals(const char *left,const char *right)
{
char a,b;
while(*left && *right)
{
a=*left;
b=*right;
if(a>=65 && a<=90) a=a+32;
if(b>=65 && b<=90) b=b+32;
if(a!=b)return 0;
left++;
right++;
}
return *left==*right; 
}
char * getMIMEType(char *resource)
{
char *mimeType=NULL;
int len=strlen(resource);
if(len<4)return mimeType;
int lastIndexOfDot=len-1;
while(lastIndexOfDot<0 && resource[lastIndexOfDot]!='.')lastIndexOfDot--;
if(lastIndexOfDot==0)return mimeType;
if(extentionEquals(resource+lastIndexOfDot+1,"js"))
{
mimeType=(char *)malloc(sizeof(char)*16);
strcpy(mimeType,"text/javascript");
}
if(extentionEquals(resource+lastIndexOfDot+1,"css"))
{
mimeType=(char *)malloc(sizeof(char)*9);
strcpy(mimeType,"text/css");
}
if(extentionEquals(resource+lastIndexOfDot+1,"html"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"text/html");
}
if(extentionEquals(resource+lastIndexOfDot+1,"jpg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extentionEquals(resource+lastIndexOfDot+1,"jpeg"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/jpeg");
}
if(extentionEquals(resource+lastIndexOfDot+1,"png"))
{
mimeType=(char *)malloc(sizeof(char)*10);
strcpy(mimeType,"image/png");
}
if(extentionEquals(resource+lastIndexOfDot+1,"ico"))
{
mimeType=(char *)malloc(sizeof(char)*11);
strcpy(mimeType,"image/x-icon");
}
return mimeType;
}
char isClientSideResource(char *resource)
{
int i=0;
for(;resource[i]!='\0' && resource[i]!='.';i++);
if(resource[i]=='\0')return 'N';
return 'Y'; //this will have to be change later
}
Request * parseRequest(char *bytes)
{
int i,j;
char method[11];
char resource[1001];
for(i=0;bytes[i]!=' ';i++)method[i]=bytes[i];
method[i]='\0';
i+=2;
int dataCount=0;
Request *request=new Request();
if(strcmp(method,"GET")==0)
{
for(j=0;bytes[i]!=' ' && bytes[i]!='?';i++,j++)resource[j]=bytes[i];
resource[j]='\0';
if(bytes[i]=='?')
{
i++;
int si=i;
dataCount=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&')dataCount++;
i++;
}
dataCount++;
int *pc=(int *)malloc(sizeof(int)*dataCount);
i=si;
int j=0;
while(bytes[i]!=' ')
{
if(bytes[i]=='&')
{
pc[j]=i;
j++;
}
i++;
}
pc[j]=i;
i=si;
j=0;
int howManyToPick;
string title;
string data;
char *d;
int x=0;
while(j<dataCount)
{
howManyToPick=pc[j]-i;
d=(char *)malloc(sizeof(char)*(howManyToPick+1));
strncpy(d,bytes+i,howManyToPick);
d[howManyToPick]='\0';
x=0;
title="";
while(d[x]!='=')
{
title+=d[x];
x++;
}
x++;
data="";
while(x<howManyToPick)
{
data+=d[x];
x++;
}
free(d);
i=pc[j]+1;
j++;
}
request->dataMap.insert(pair<string,string>(title,data));
}
}//if ends// method is of GET type
request->method=(char *)malloc((sizeof(char)*strlen(method))+1);
strcpy(request->method,method);
if(resource[0]=='\0')
{
request->resource=NULL; 
request->isClientSideTechnologyResource='Y';
request->mimeType=NULL;

}
else
{
printf("request arrived for %s\n",resource);
request->resource=(char *)malloc((sizeof(char)*strlen(resource))+1);
strcpy(request->resource,resource);
request->isClientSideTechnologyResource=isClientSideResource(resource);
request->mimeType=getMIMEType(resource);
}
return request;
}

TMWebProjector::TMWebProjector(int portNumber)
{
this->portNumber=portNumber;
}
TMWebProjector::~TMWebProjector()
{
//if(this->url)delete [] this->url;
}
void TMWebProjector::onRequest(char *url,void (*p)(Request &,Response &))
{
//int len=strlen(url);
//char *url=new char[len+1];
string s=url;
//strcpy(this->url,url);
urls.insert(pair<string,void (*)(Request &,Response &)>(s,p));
}
void TMWebProjector::start()
{
FILE *f;
int length;
int i,rc;
char g;
int len;
char requestBuffer[8193];
char responseBuffer[1025];
int bytesExtracted;
WSADATA wsaData;
WORD ver;
int serverSocketDescriptor;
int clientSocketDescriptor;
int successCode;
struct sockaddr_in serverSocketInformation;
struct sockaddr_in clientSocketInformation;
ver=MAKEWORD(1,1);
WSAStartup(ver,&wsaData);
serverSocketDescriptor=socket(AF_INET,SOCK_STREAM,0);
if(serverSocketDescriptor<0)
{
printf("Unable to create socket\n");
return;
}
serverSocketInformation.sin_family=AF_INET;
serverSocketInformation.sin_port=htons(this->portNumber);
serverSocketInformation.sin_addr.s_addr=htonl(INADDR_ANY);
successCode=bind(serverSocketDescriptor,(struct sockaddr *)&serverSocketInformation,sizeof(serverSocketInformation));
if(successCode<0)
{
printf("Unable to bind socket to port %d\n",this->portNumber);
WSACleanup();
return;
}
listen(serverSocketDescriptor,10);
len=sizeof(clientSocketInformation);
while(1)
{
printf("server is ready to accept request on port %d\n",this->portNumber);
clientSocketDescriptor=accept(serverSocketDescriptor,(struct sockaddr *)&clientSocketInformation,&len);
if(clientSocketDescriptor<0)
{
printf("unable to accept client connection\n");
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}
bytesExtracted=recv(clientSocketDescriptor,requestBuffer,8192,0);
if(bytesExtracted<0)
{
printf("bytesExtracted<0\n");
// what to do is yet to be decided;
}else if(bytesExtracted==0)
{
printf("bytesExtracted==0\n");
// what to do is yet to be decided;
}else
{
requestBuffer[bytesExtracted]='\0';
Request *request=parseRequest(requestBuffer);
while(1) // infinite loop to enable forwarding feature
{
if(request->resource!=NULL)
{
char *resource=request->resource;
const char *ptr=resource+(strlen(resource)-1);
if(*ptr=='t' &&*(ptr-1)=='c' && *(ptr-2)=='s' && *(ptr-3)=='.')
{
f=fopen(resource,"r");
char c;
FILE *t=fopen("tmp.data","w");
char a[100];
char stringValue[15];
int l,value;
while(1)
{
c=fgetc(f);
if(feof(f))break;
if(c=='$')
{
fgetc(f);
l=0;
while(1)
{
c=fgetc(f);
if(c=='}') break;
a[l]=c;
l++;
}// inner infinite loop ends
c=fgetc(f);

a[l]='\0';
value=request->getInt(a);
sprintf(stringValue,"%d",value);
fputs(stringValue,t);
}//if ends
fputc(c,t);
}
fclose(f);
fclose(t);
f=fopen("tmp.data","rb");
printf("sending %s\n",resource);
fseek(f,0,2); // move the internal pointer to the end of file 
length=ftell(f);
fseek(f,0,0);//move the internal pointer to the start of the file
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection: close\n\n",request->mimeType,length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
int k=0;
while(k<length)
{
rc=length-k;
if(rc>1024)rc=1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
k+=rc;
}
fclose(f);
break;// indroduce because of forwarding feature
}//extention compare if ends
}
if(request->isClientSideTechnologyResource=='Y')
{
if(request->resource==NULL)
{
f=fopen("index.html","rb");
if(f!=NULL)printf("sending index.html\n");
if(f==NULL)
{
f=fopen("index.htm","rb");
if(f!=NULL)printf("sending index.htm\n");
}
if(f==NULL)
{
printf("sending 404\n");
strcpy(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent=Length:234\nConnection: close\n\n<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource / not found</h2></body></html>");
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
break;// indroduce because of forwarding feature
}else
{
fseek(f,0,2); // move the internal pointer to the end of file 
length=ftell(f);
fseek(f,0,0);//move the internal pointer to the start of the file
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length:%d\nConnection: close\n\n",length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-i;
if(rc>1024)rc=1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
break;// indroduce because of forwarding feature
}
}else
{
f=fopen(request->resource,"rb");
if(f==NULL)
{
char tmp[501];
printf("sending 404\n");
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent=Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
break;// indroduce because of forwarding feature
}else
{
printf("sending %s\n",request->resource);
fseek(f,0,2); // move the internal pointer to the end of file 
length=ftell(f);
fseek(f,0,0);//move the internal pointer to the start of the file
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:%s\nContent-Length:%d\nConnection: close\n\n",request->mimeType,length);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
i=0;
while(i<length)
{
rc=length-i;
if(rc>1024)rc=1024;
fread(responseBuffer,rc,1,f);
send(clientSocketDescriptor,responseBuffer,rc,0);
i+=rc;
}
fclose(f);
break;// indroduce because of forwarding feature
}
}
}else
{
// what to do in case of server side resource , is yet to be decided
int ii=0;
//if(this->url[0]=='/')ii=1;
map<string,void (*)(Request &,Response &)>::iterator i;
string url=request->resource;
i=this->urls.find(url);
if(i==urls.end())
{
char tmp[501];
printf("sending 404\n");
sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s not found</h2></body></html>",request->resource);
sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent=Length:%d\nConnection: close\n\n",strlen(tmp));
strcat(responseBuffer,tmp);
send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
break;// indroduce because of forwarding feature
}
else
{
Response response(clientSocketDescriptor);
i->second(*request,response);
if(request->tellIsRequestForwarded())
{
free(request->resource);
request->resource=(char *)malloc(sizeof(char)*strlen(request->forwardedRequest));
strcpy(request->resource,request->forwardedRequest);
request->isClientSideTechnologyResource=isClientSideResource(request->resource);
request->mimeType=getMIMEType(request->resource);
continue;
}
//done
//char tmp[501];
printf("sending processed\n");
//sprintf(tmp,"<DOCTYPE HTML><html lang='en'><head><meta charset='utf-8'><title>TM Web Projector</title></head><body><h2 style='color:red'>Resource /%s processed</h2></body></html>",request->resource);
//sprintf(responseBuffer,"HTTP/1.1 200 OK\nContent-Type:text/html\nContent=Length:%d\nConnection: close\n\n",response->getLengthOfResponse());
//strcat(responseBuffer,response->getResponse().c_str());
//send(clientSocketDescriptor,responseBuffer,strlen(responseBuffer),0);
}
}
break; //introduced because of the forwarding feature
}//infinite loop introduced because of forwarding feature is ends here
}
closesocket(clientSocketDescriptor);
}// infinite loop related to accept method end here
closesocket(serverSocketDescriptor);
WSACleanup();
return;
}