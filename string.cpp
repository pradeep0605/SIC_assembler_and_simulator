/*THis program includes 
   1. Operator overloading for reading and writing an object from console
   2. Multiple types of constructors
   3. Dynamic memory allocation
   4. Illustration of THIS pointer
   5. Nesting of member functions
   6. Anonymous object creation.  (NOt possible in turbo c++)
   7. Writing of String object to file as text.
   8. Error handling. Exception handling (NOt possible in turbo c++)
   9. Function overloading
  10. Conversion from class to basic 
  11. Conversion from basic to class
  12. single Operator cascading.
  13. Multiple Operator cascading. 
  14. Extensive string hadling functions defined.
*/

#include<iostream>
#include<fstream>
#include<cstring>
#include<cstdlib>
#include<exception>

using namespace std;

class String
{

  char delim; 					 //Deliminter for reading the string from the console		
  int length;
  int    find(String,char);					 //Stores the length of the string
  
  public:
       char *str;                    		 //Character array
       short  instructionAddress;  //Only used as a special data member for  sic assembler	
     //Default constructordd
     //default delimiter is newline character. i.e Reads the string till the new 
     String()
     {
       instructionAddress=length=0;
       str=new char[length+1];
       delim='\n';
       
     }
     
     //constructor which constructs(memory allocation) the string with the specified no of character size.
     String(int size)
     {
       length=size;
       str=new char[length+1];
       delim='\n';
     }
     
     //Constructor what takes String literals to construct the object. 
     String(char *s)
     {
       length=strlen(s);
       str=new char[length+1];
       strcpy(str,s);
       delim='\n';
     }
     
     //this funtion sets the deliminter for reading the string from the key borad
     void setDelimiter(char ch)
     {
       delim=ch;            
     }
     
     //Retruns the length of the String object. (no of character exculding '\0' character
     int getLength()
     {
        this->length=strlen(str);
        return length;
     }
     
     //WRITING STRING TO CONSOLE "<<" overloaded
     friend ostream & operator <<(ostream &out,String s)
     {
          out<<s.str;
          return out;       
     }
     
     //WRITING STRING TO FILE 
     friend ofstream& operator <<(ofstream &out, String s)
     {
        out<<s.str;
        return out; 
     }     
     
     //READING STRING FROM FILE
     friend ifstream & operator >>(ifstream &fin,String &s)
     { 
         s=String(1024);
         fin>>s.str;
         return fin;
     }

     //Converts the string object to character array (Returns the address of the pointer which points to the
     //                                               beggining of the character array)                         
     char * toCharArray()
     {
        return str;
     }

     //SUBSTRING 1 TYPE (which gives string from the specified location till the end of string)
     String subString(int index)
     {
       if(index>strlen(str) || index<0)
       {
         throw ("Array out of bound exception!"); // if index is out of range(array bound cheking).
       }
       return *new String(&str[index]);
     }
     
     
     //SUBSTRING 2 TYPE (which gives the substring from the specified starting location 
     //		         to the location specified by the "end" variable			
     String subString(int start,int end)
     {
       if( start<0 || end > strlen(str)  || start>end)
       {
         throw("Array out of bound exception!");//Array bound cheking.
       }
      
       String s(this->getLength());
       strcpy(s.str,str);
       s.str[end+1]='\0';
       s.str=&s.str[start];
       return s;
     }
     
     // Function prototypes for which definition is available outside the class.     
     friend istream & operator >>(istream &in,String &s);
     operator int();    
     String operator + (char *);
     String operator + (String);
     String operator - (char *);
     String operator - (String);
     int    operator ==(String);
     int    operator ==(char *);
     int   toInteger();
     void   operator =(int);
     String * tokenize(char,int&);  
     int indexOf(String );
     String trim(char ch=' ');
};

   //Function for searching a string in the text..
   int String::indexOf(String pattern)
   {   int j=0;
       if(pattern.getLength() > this->getLength())
       {
          return -1;      // Error that the text to be searched cannot be more than the text.
       }
        for(int i=0;i<=this->getLength()-pattern.getLength();i++)
        {
           j=0; //To start searching pattern in the text from beggining of pattern.(text inedex is continued).
           while(str[i]==pattern.str[j] && j<pattern.getLength())//till characters are successively found and till j< patterns's length
           {
             i++;
             j++;
           }         
           if(j==pattern.getLength())
           {
             return i-j; //return the location where the character found.
           }
          
        }
      return -1;  //NO match found 
   }
   
    // This function is used to remove the unwanted character and tabspace 
    //    before and after the string. (it also removes the tab spaces in between the text, but not the spaces *)...
   String String:: trim(char ch)
   {
      String temp=*this;
      for(int i=0;i<temp.getLength();i++)
      {     
          
            if(temp.str[i]=='\t')
            {
              temp.str[i]=ch;          //replacing tabspace by single space.
            }
      }
      int beg=0;
      char chr=temp.str[beg];
      while(chr==ch)    // This code(while part) is used to remove the blank spaces at the beggining.
      {
        beg++;
        chr=temp.str[beg];  
       }
      temp=temp.subString(beg);
      
      int end=temp.getLength()-1;
      chr=temp.str[temp.getLength()-1];
      while(chr==ch) // Used to remove the blank spaces at the end.
      {
        end--;
        chr=temp.str[end];
      }   
      temp=temp.subString(0,end);
      *this=temp;
      return *this;
   }
   
   
   // One of the most widely used string fucntions
   /* This function is used to tokenize(create fragments or parts) the given string    
      into array of string with the given deliminter. i.e "ch" here. (excluding delimiter).
   */
  
   //READING STRING FROM CONSOLE
     istream & operator >>(istream &in,String &s) 
     {    char ch,temp=s.delim;
          s=String(1024);
          s.delim=temp;
          int i=0;
          while((ch=getchar())!=s.delim)//read the character till the delimiter character.
          { 
             s.str[i]=ch;
             i++;
          }
           s.str[i]='\0';
          s.length=strlen(s.str);
          return in;
     }

 void stringReverse(char *s)
 {
      char temp[strlen(s)+1];
       int i=0;
      for(i=0;i<strlen(s);i++)
      {
         temp[i]=s[strlen(s)-1-i];
      }
      temp[i]='\0';

      for(i=0;i<strlen(s);i++)
      {
          s[i]=temp[i];
      }
              
 }
 
    //Conversion from basic to class type
    void String::operator=(int i)
   {
      String temp=String(20);
      int rev=0,mul=10;
      temp.str[0]='\0';
      int k=0;
      while(i>0)
      {
         if(!((i%10)+48)>=48 &&  ((i%10)+48)<=57) //cheking whether the character is not b/w 30h and 39h, if so throw the error.
         {
	    //cout<<"Conversion exception\n Invalid character occurance while conversion\n"<< i<<endl;
    	    throw("Conversion exception\n Invalid character occurance while conversion\n");
         }
         temp.str[k]= (i%10)+48;
         i/=10;
         k++;
      }
      /* i=0;
      while(rev!=0)
      {
        temp.str[i]= char((rev%10) + 48); //converts integer to string.
        rev/=10;
        i++;
      }
      
      temp.str[i]='\0';
      */
      temp.str[k]='\0';
      stringReverse(temp.str);
      *this=temp;
   }       
   //converts from string to integer.
   int String :: toInteger()
   {
       char *temp=str;
       int res=0,mul=1;
       while(*temp!='\0')
       {
         temp++;
       }
       temp--;
       
       while(temp!=(str-1))
       {
          if(int(*temp==7))
          {
            temp--;
            continue;
          }
          if( int(*temp) <48 || int(*temp) >57) //checking for non numerical character if found throw an exception.
          {
            throw ("Invalid string to integer conversion\n Invalid characters found\n");
          }
          else
          {
             res=res+ mul*(int(*temp)-int('0'));
             mul*=10;
          }
          temp--;
       }
	
	return res;
   }
   
   
   String :: operator int()
   {
       return this->toInteger();
   }
   
  //For string concatination with string literal 
 String String:: operator + (char *s)
{ 
  strcat(str,s);
  return *this; 
}

//For String concatination with another string object.
String String:: operator +(String s)
{
  strcat(str,s.str);
  return *this;
}


//For deleting the character that are there in *s from the string object
String String::operator -(char *s)
{
  String temp(this->getLength());
  int j=0;
  
  String del(s);
  
    for(int i=0;i<this->getLength();i++)
    {
        if(find(del,str[i]))
        {
          continue;
        }     
        else
        {
           temp.str[j]=str[i];
           j++;  
        }
    }
    temp.str[j]='\0';
    temp.length=strlen(temp.str);
  return temp;
}

//Finds a character in a given string. (used for only internal operational purpose and is hidden to user.
//As they have indexOf function for searching.
int String:: find(String s,char ch)
{
  
    for(int i=0;i<s.getLength();i++)
    {
       if(s.str[i]==ch)
          return 1;
    }
    return 0;
}

//For deleting the character that are there in string object s from the string object
String String::operator -(String s)
{
  return *this- s.str; // NEsting of member functions
}

int String:: operator == (String s)
{
   if(this->getLength()!= s.getLength())
   {
      return 0;
   }
   
   for(int i=0;i<s.getLength();i++)
   {
      if(this->str[i] != s.str[i])
      { 
           return 0;
      }
   }
  return 1;
}

int String:: operator ==(char *s)
{
   
   if(this->getLength()!= strlen(s))
   {
      return 0;
   }
   
   for(int i=0;i<strlen(s);i++)
   {
      if(this->str[i] != s[i])
      { 
           return 0;
      }
   }
  return 1;
}

int countTokens(char *str,char delimiter)
{
   char ch=0;
   int no_tokens=0;
   int k=0;
   ch=str[k];
   while(ch!='\0')
   {
       while(ch==delimiter)
          ch=str[++k]; 

       if(ch=='\0')    
         break;
         
       no_tokens++;
         
       while(ch!=delimiter && ch!='\0')
          ch=str[++k];

       if(ch=='\0')
         break;
   }
   return no_tokens;
}

String * String::tokenize(char delimiter,int &n)
{
     n=countTokens(str,delimiter);
     if ( n==0 ) 
       return NULL;
       
     String *tokens=new String[n+1];
     unsigned int start=0,i=0,end=0,k=0,  tokenIndex=0;
     char ch=0;
     ch=str[k];
     
     while(ch!='\0')
     {
                 while(ch==delimiter)
                     ch=str[++k];
                     
                 if(ch=='\0')
                     break; 
                    
                    start=k;
                    
                 while(ch!=delimiter && ch!='\0')
                    ch=str[++k];
                 end=k;
                   
                 tokens[tokenIndex].str=new char[end-start+2];                  
                 for(i=0;i<end-start;i++)
                      tokens[tokenIndex].str[i]=str[i+start];
                 tokens[tokenIndex].str[i]='\0';
                 tokenIndex++;         
     }
   return tokens;
}

/*   String *  String:: tokenize(char ch,int &n)
   {
      if(getLength()==0)//checking for empty string
      { 
         n=0;
         return 0;
      } 
      String temp(this->getLength());
      
      strcpy(temp.str,this->str);
      temp.trim(ch);
      int count=0,j=0,i,k;
      int indexes[temp.getLength()];

      for(i=0;i<temp.getLength();i++)
      {
        if(temp.str[i]==ch)
        {
          count++;
          indexes[j]=i;          //count the number of occurance of the delimiter.
	  j++;
	  while(temp.str[i]==ch) 
	     i++;
        }
      }
      indexes[j]=temp.getLength()-1;//assign even the last character's index.
      
      String *token=new String[count];//create array of string for the number of tokens.
      
      for(k=0;k<=j;k++)
      { 
        try
        {
         if(k==0)
         {
            token[k]=temp.subString(0,indexes[k]-1);        // assign the string(as token) from begging to the first occurance
         }                                                  // of the delimiter.
         else
         if(k==j)
         {
           token[k]=temp.subString(indexes[k-1]+1,indexes[k]); //assign the string from last occurance of the delimiter till the end
         }
         else
         {
            token[k]=temp.subString(indexes[k-1]+1,indexes[k]-1);//assign the string b/w two deliminters.
         }
        }
        catch(const char *s)
        {} 
      }
     //There will be many occurance of successive delimiters. So for them empty tokens will be
     // create. SO they should be removed.      
     int fin_count=0;
     
     for(k=0;k<=j;k++)
     {
        if(token[k].getLength()!=0)
        {
          fin_count++;   //count only the tokens which have length more thatn 0.
        }
     }
       
     String *fin_token=new String[fin_count]; //create array of string only for the valid tokens. 
     i=0;
     for(k=0;k<=j;k++)
     {
       if(token[k].getLength()!=0)
       {
          token[k].trim(ch);
          fin_token[i]=token[k];
          i++;
       }
     }
     n=fin_count;
     return fin_token;
   }
*/















