#include "otherFunctions.cpp"       
#include <fstream>


/*

INCLUDED IN THE 
class lineTokens
{
    public:
    String *tokens;
    int n;
    lineTokens()
    {
       tokens=NULL;
       n=0;
    }
};
*/

void insertLineNumbers(ifstream &in, ofstream &out)
{
  char ch=0;
  int count=1;
  String s;
  s=count;
  count++;
  out.write((char*)s.str,strlen(s.str));
  ch=' ';
  out.write((char*)&ch,1);
  int fileSize=getSize(in);
  
  while(fileSize>0)
  {
     in.read((char*)&ch,1);
     fileSize--;
     if(ch=='\n' && fileSize>0)// To check for newline character and inserting the line number after that
     {                         // and the second condition is used for not to insert line number after end of file
        /* this code inserts line number and a space for each line*/
        out.write((char*)&ch,1);
        s=count;
        count++;
        out.write((char*)s.str,strlen(s.str));
	ch=' ';	
	out.write((char*)&ch,1);
     }
     else
         out.write((char*)&ch,1);// else simply copy the other contents
  }
}

/* This function not only removes comment, but also gives before and after the ',' (comma) and
for each line line numbers are prefixed (using insertLineNumbers function)*/
void removeComments(char *s)  
{
   ifstream ins(s,ios::binary);
   ofstream outs("lineInserted.sicout",ios::binary);
   ifstream in("lineInserted.sicout",ios::binary);
   ofstream out("first.sicout",ios::binary);
   
   insertLineNumbers(ins,outs); 
   outs.close();
      
   int fileSize=getSize(in);
   char ch=0;
   while(fileSize>0)
   {
       in.read((char*)&ch,1);
       fileSize--;
       if(ch==';')
       {
          while(ch!='\n')
          {
             in.read((char*)&ch,1);
             fileSize--;
          }
       }
       if(ch==',')
       {
          
          out.write((char*)&(ch=' '),1);
          out.write((char*)&(ch=','),1);
          out.write((char*)&(ch=' '),1);          
       }
        out.write((char*)&ch,1);

   }
}

void convertToSmallCase(char *s)
{
   ifstream in(s,ios::binary);
   ofstream out("second.sicout",ios::binary);
   int fileSize=getSize(in);
   char ch=0;
   while(fileSize>0)
   {
      in.read((char*)&ch,1);
      if(ch=='"')
      {
         out.write((char*)&ch,1);
         in.read((char*)&ch,1);
         fileSize--;
         while(ch!='"' && fileSize>0)
         {

                 out.write((char*)&ch,1);
		 in.read((char*)&ch,1);
		 fileSize--;
         }          
      }
      if((int)ch>=65 && (int) ch<=90)
      {
        ch+=32;
      }
      out.write((char*)&ch,1);
      fileSize--;
   }
}

void addAllLabels(String *lines, int numberOfLines, lineTokens *LT, optab &opcodeTab, symtab &symbolTab,bool &error)
{
    char ch=0;
   ofstream out("third.sicout",ios::binary);
   for(int i=0;i<numberOfLines;i++)
   {
      int n=0;
         
      if(!emptyLine(lines[i])) // if not an empty line 
      {
         String *individualTokens=LT[i].tokens; /*each line is tokenized and each tokenized tokens are      	                                                     written in   to the file                */
         for(int j=0; j<n; j++)
         {
           out.write((char*)individualTokens[j].str,individualTokens[j].getLength());
           ch=' '; // after each token is written a single space is given.
           out.write((char *)&ch,1);
         }
      }
      ch='\n';
      out.write((char *)&ch,1);
   }
   out.close();

     for(int i=0;i<numberOfLines;i++)
     {
        if(!emptyLine(lines[i]))
        {
           int n=0;
           String *individualTokens=lines[i].tokenize(' ',n);               
           
               if( opcodeTab.find(individualTokens[1])) // initially search for the token in optab
               {
                  continue;
               }
               else
               if( ! symbolTab.find(individualTokens[1]) )// then search for the token in symtab. If not found add it 
               {

                    symbolTab.addSymbol(individualTokens[1]); //adding the token as a symbol

                    if( ! opcodeTab.find(individualTokens[1+1]))// if the next token not found as an opcode then it is an error 
                    {                                   // So remove the symbol added just now ..
		      cout<<"line "<<individualTokens[0]<<" :"<<" Invalid Opcode in the instruction:\t\""<<lines[i]<<"\""<<endl;
                      error=true; 
                      symbolTab.removeRecentlyAdded();
                      continue;
                    }
               }
               else // if found then a symbol of that token already exists!! hence error
               {
                  error=true;
                  cout<<"line "<<individualTokens[0]<<" :"<<" Ambiguous use of symbol \""<<individualTokens[1]<<"\" in the instruction :\t\""<<lines[i]<<"\""<<endl ;
               }
           
        }
     }
}

void trimTheFile(char *s)
{
    ifstream in(s,ios::binary);
    ofstream out("trimmed.sicout",ios::binary);
    int fileSize=getSize(in);
    char ch=0;
    while(fileSize>0)
    {
       in.read((char*)&ch,1);
       fileSize--;
       if(ch=='\t')
         ch=' ';
       out.write((char*)&ch,1);
    }
    
    out.close();
    
}

int main(int argc, char **argv)
{  
  if(argc<2)
  {
     cout<<"Invalid number of arguments!!";
     return 0;
  }

  bool error=false;
    
  trimTheFile(argv[1]); // Removes the tabs (replaces tabs with spaces
  removeComments((char*)"trimmed.sicout");// Remove comments and assign line numbers
  convertToSmallCase((char*)"first.sicout");// Convert entire program to small case letters
  
     
  optab opcodeTab=optab();  // Creation of optab and symtab. Here optab is static and automatically initializes itself.
  symtab symbolTab=symtab(); //
  //opcodeTab.display();
  
  ifstream in("second.sicout",ios::binary); 
  int numberOfLines=0;
  String code=String( getSize(in) );
  in.read((char*)code.str,getSize(in)); // read the entire file
    
  String *lines=code.tokenize('\n',numberOfLines); // Tokenizing entire program into number of lines using delimiter as \n
    
  
  
  
           cout<<"no error\n";
           cout.flush();
  lineTokens *LT=new lineTokens[numberOfLines];
  
  for(int i=0;i<numberOfLines;i++)
  {
      if(!emptyLine(lines[i]))
      {
           LT[i].tokens=lines[i].tokenize(' ',LT[i].n);
      }  
  }    
  
  addAllLabels(lines,numberOfLines,LT,opcodeTab,symbolTab,error);//Adding all the labels to the symbol table. Not only just findnig the
 // labels but also generate error whether the labels are defined ambiguously and even searches for invalid opcodes.++

  symbolTab.display();

  
   generateAddressesOfInstructions(lines,numberOfLines,LT,opcodeTab,symbolTab); /* Generating the addresses for each instruction
   and the address is assigned to each instruction' (in its string (lines)) instructionAdresss  variable */
  
  
  for(int i=0;i<numberOfLines;i++)
  {
     if(! emptyLine(lines[i]))
     {
        cout<<lines[i].instructionAddress<<"\t\t"<<lines[i]<<endl;
     }   
  }

  assignAddressToLabels(lines,numberOfLines,LT,symbolTab);

  
  cout<<endl<<"ADDRESSES OF LABELS"<<endl;
  
   for(int i=0;i<symbolTab.numberOfLabels;i++)
   {
          cout<<symbolTab.labels[i]<<" =  "<<symbolTab.labelAddresses[i]<<endl;
   }

   
   
   generateMachineCode(lines,numberOfLines,LT,opcodeTab,symbolTab,argv[1],error);
  
    return 0;
   //system("rm lineInserted.sicout first.sicout second.sicout third.sicout");
}





