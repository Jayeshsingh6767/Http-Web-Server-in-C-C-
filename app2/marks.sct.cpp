#include<tmwp>
using namespace tmwp;
void getMarks_SCT(Request &request,Response &response)
{
response.write("<!DOCTYPEHTML><HTMLlang='en'><head><metacharset='utf-8'></head><body>Physics=");
response.write(request.get("physics");
response.write("<br>chemistry=");
response.write(request.get("chemistry");
response.write("<br>maths=");
response.write(request.get("maths");
response.write("<br>english=");
response.write(request.get("english");
response.write("<br>hindi=");
response.write(request.get("hindi");
response.write("<br></body></html>
}