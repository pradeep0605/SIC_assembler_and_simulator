#include<iostream>
#include<stdio.h>


using namespace std;

int main()
{ 
   char ch=0;
   char arr[100];
   int k=0;
  while((ch=getchar())!='\n')
  arr[k++]=ch;
  arr[k]='\0';
  
    cout<<arr<<endl;
   return 0;
}
 
