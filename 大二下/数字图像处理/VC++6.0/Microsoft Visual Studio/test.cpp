#include<iostream.h>
void main()
{
   int i,count,*p;
   cin>>count;
   p=new int[count];
   for(i=0;i<count;i++)
      cin>>p[i];
   for(i=0;i<count;i++)
      cout<<p[i]<<" ";
   delete []p;
}
