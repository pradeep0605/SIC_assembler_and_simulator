#include "optab.cpp"

class symtab
{
   public:
   
   String *labels;
   int numberOfLabels;
   short *labelAddresses;
   
   symtab()
   { 
       numberOfLabels=0;
       labels=new String[150];
       labelAddresses=new short[150];
       for(int i=0;i<150;i++)
       {
          labelAddresses[i]=0;
       }
   }

   symtab(int n)
   {
      numberOfLabels=0;
      labels=new String[n];
   }

   int find(String s)
   {
      for(int i=0;i<numberOfLabels;i++)
          if(labels[i]==s)
            return 1;

      return 0;      
   }

  short getLabelAddress(String s)
  {
      for(int i=0;i<numberOfLabels;i++)
      {   
         if(labels[i]==s)
           return labelAddresses[i];
      }
     return 0;
  }
  
   void addSymbol(String s)
   {
        labels[numberOfLabels++]=s;
   }

   void removeRecentlyAdded()
   {
     numberOfLabels--;
   }
   
   void display()
   {
      for(int i=0;i<numberOfLabels;i++)
      {
         cout<<labels[i]<<" ";
      }
      cout<<endl;
   }

   
};



