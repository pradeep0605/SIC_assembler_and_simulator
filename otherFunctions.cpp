#include "symtab.cpp"

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

class instruction
{
   public:
   unsigned char opcode,op1,op2;
   instruction()
   {
      opcode=op1=op2=0;
   }

   void setInstruction(unsigned char opcd, unsigned short opr)
   {
       opcode=opcd;
       op2=opr%256;
       opr/=256;
       op1=opr;
   }
   unsigned short operand()
   {
      unsigned operand=0;
      operand=(short)op1*256;
      operand+=op2;
      return operand;
   }
};



int emptyLine(String s)
{
  int k=0;
  while(s.str[k]>=48 && s.str[k]<=57)
     k++;

  while(k<strlen(s.str))
  {

     if(s.str[k]==' ' || s.str[k]=='\t')
     {
        k++;
        continue;
     }
     else
        return 0;
  }
  return 1;
}


void assignAddressToLabels(String *lines, int numberOfLines,lineTokens *ST, symtab &symbolTab)
{
     for(int i=0;i<symbolTab.numberOfLabels;i++)// For each label search in all the instructions
     {
          for(int j=0;j<numberOfLines;j++)
          {  
             if(!emptyLine(lines[j]))
             {
                      int n=0;
		      String *tokens=ST[j].tokens;
		      n=ST[j].n;
		        
		      if(tokens[1]==symbolTab.labels[i])
		      {
		         symbolTab.labelAddresses[i]= lines[j].instructionAddress;
		      }
             }    
          }
     }
}


//Function for generating addresses to each instruction
void generateAddressesOfInstructions(String *lines, int numberOfLines,lineTokens *LT,optab &opcodeTab, symtab &symbolTab)
{
    bool error=false;
    int locationCounter=0;
    for(int i=0;i<numberOfLines;i++)
    { 
        if(! emptyLine(lines[i]))
        { 
           int n=0;
           
           String *individualTokens=LT[i].tokens;
           n=LT[i].n;

           if(lines[i].indexOf((char*)"clear") !=-1 ||  lines[i].indexOf((char*)"tixr") !=-1 )
           { 
                   lines[i].instructionAddress=locationCounter; // simply assign the location counter vlaue
                   locationCounter+=3; 
                   continue;
           }
           
           if(individualTokens[1]==(char*)"end" || individualTokens[1]==(char*)"rsub" )// Check for end or rsub or read or disp then return only opcode instructions.
           {
              lines[i].instructionAddress=locationCounter;
              return;
           }

           if(individualTokens[1]==(char*)"disp" || individualTokens[1]==(char*)"read" )
           {  
              lines[i].instructionAddress=locationCounter; // simply assign the location counter vlaue
              locationCounter+=3; 
              continue;
           }

           if(individualTokens[1]==(char*)"start" || individualTokens[2]==(char*)"start")// if its a start assembler directive
           { 
               lines[i].instructionAddress=locationCounter;
               locationCounter+=0;
               continue;
           }
         
           //if it is a normal instruction other than "RESB" "RESW" "BYTE" "WORD"
           
           String temp=individualTokens[2];
           if(  temp==(char*)"resb" || temp==(char*)"resw" || temp==(char*)"byte" || temp==(char*)"word" )
           { 
		       if(individualTokens[2]==(char*)"resw")// if opcode is a "reserve word" then increment according to the size given
		       { 
			   lines[i].instructionAddress=locationCounter;
		           int size=individualTokens[3];
		           locationCounter= size*3 + locationCounter;
		       }
		       
		      if(individualTokens[2]==(char*)"resb")// if opcode is a "reserve byte" then increment according to the size given
		       { 
			   lines[i].instructionAddress=locationCounter;
		           int size=individualTokens[3];
		           locationCounter= size + locationCounter;
		       } 
		       
		       if(individualTokens[2]==(char*)"word")// if opcode is a "word" then increment according to the size given
		       { 
			   lines[i].instructionAddress=locationCounter;
		           int size= (n-3)/2 + 1 ;
		           locationCounter= size * 3 + locationCounter;
		       }

		       if(individualTokens[2]==(char*)"byte")// if opcode is a "byte" then increment according to the constant(s) given
		       { 
		           if(individualTokens[3].str[0]=='"')
		           {
		                 lines[i].instructionAddress=locationCounter;
		                 int escapeCount=0;
		                 for(int i=1;i<individualTokens[3].getLength()-1;i++)//To count the number of escape characters,
		                 {                                                   //to eliminate them in the length of the variable
		                    if(individualTokens[3].str[i]=='\\')
		                       escapeCount++;
		                 }
		                 int size= individualTokens[3].getLength()- escapeCount - 2; // -2 because just to leave two double quotes "" 
 			         locationCounter= size + locationCounter + 1;//To indicate the inclusion of '\0' at the end
		           }
		           else
		           {
				   lines[i].instructionAddress=locationCounter;
				   int size= (n-3)/2 + 1; 
				   locationCounter= size + locationCounter;
                           }
		       }
           }
           else 
           {
               
           
                lines[i].instructionAddress=locationCounter; // simply assign the location counter vlaue
                locationCounter+=3;                          //increment it by three
           }          
           
        }//end of if not a empty line
    }// for loop
}

int isRegisterInstruction(String s)
{
   if(s==(char*) "addr" || s==(char*) "clear" || s==(char*) "compr" || s==(char*) "divr" || s==(char*) "mulr" || s==(char*) "rmo" || s==(char*) "subr" || s==(char*) "tixr")
     return 1;
   else
     return 0;
}


void generateMachineCodeForMemoryInstructions(String *tokens,int n,ofstream &out, bool &error)
{


  unsigned int  inst=0;
  if(tokens[2]==(char*)"resw")
  {
     if(n>4) //only four tokens should be found 1(linenumber) 2(label) 3(resw) 4(size). if any other things are there then its an error
     {
              cout<<"line "<<tokens[0]<<" : "<<" invalid syntax of the instruction :\n";
    	      error=true;
	      return ;   
     }
     
     int k=tokens[3];
     for(int i=0;i<k;i++)
       out.write((char*)&inst,3); // three bytes
  }

  if(tokens[2]==(char*)"resb")
  {
    
     if(n>4) //only four tokens should be found 1(linenumber) 2(label) 3(resb) 4(size). if any other things are there then its an error
     {
              cout<<"line "<<tokens[0]<<" : "<<" invalid syntax of the instruction :\n";
    	      error=true;
	      return ;   
     }
    char ch=0;
     int k=tokens[3];
     for(int i=0;i<k;i++)
       out.write((char*)&ch,1);
  }
  
  if(tokens[2]==(char*)"word")
  {
     if(n%2==1)//chechking whether the number of tokens are even or not. If odd means the constants are not specified properly like this
     {         // a word 10, 20, where , is also taken as a token and no contant after that ',' comma hence an error
        cout<<"line "<<tokens[0]<<" : "<<" invalid syntax of the instruction : \n";
        error=true;
        return ;
     }
     for(int i=3;i<n;i+=2)
     {
       int word=tokens[i];
		if(word>16777215)
                {
                   cout<<"line "<<tokens[0]<<" : "<<" overflow!! value cannot fit within a word : \""<<tokens[i]<<"\"\n";
                   error=true;
                   return;
                }
       out.write((char*)&word,3);       
     }

  }

  if(tokens[2]==(char*)"byte")
  {
	if(tokens[3].str[0]=='"')// If a string encoted within double quotes then
        {
        
           if( tokens[3].str[tokens[3].getLength()-1]!='"' )
           {
               cout<<"line "<<tokens[0]<<" : "<<" Missing '\"' :\n";
	       error=true;
	       return ;
           }
           
           char ch=0;
           for(int i=1;i<tokens[3].getLength()-1;i++) //till last " (double quote) i.e, end of string.
           {
             if(tokens[3].str[i]=='_')// If underScore is there then take it as a space
             {
               ch= ' ';
               out.write((char*)&ch,1);
             }
             else
             if(tokens[3].str[i]=='\\')//if escape character
             {
                i++;
                ch='\\';
                if(tokens[3].str[i]=='n') //if new line character 
                {
                  ch=10;
                  //out.write((char*)&ch,1);
                  //ch=13;
                }
                if(tokens[3].str[i]=='t')// if tab space character
                 ch=9;
                if(tokens[3].str[i]=='_')// if need to display underScore character
                 ch=95;
                if(tokens[3].str[i]=='"')// if need to display double quotes character
                 ch=34;

                if(tokens[3].str[i]=='b')// if need to display double quotes character
                 ch='\b';
                
                if(tokens[3].str[i]=='r')// if need to display double quotes character
                 ch='\r'; 
                
                out.write((char*)&ch,1);
             }
             else //other wise simply write remaining characters
             {
                ch=tokens[3].str[i];
                out.write((char*)&ch,1);
             }
           } 
           ch='\0';
           out.write((char*)&ch,1);
        }
        else//other wise simply write the values given in the form like this one bte 10, 20, 30, 40
        {
             if(n%2==1)//chechking whether the number of tokens are even or not. If odd means the constants are not specified properly like this
	     {         // a byte 10, 20, where , is also taken as a token and no contant after that ',' comma hence an error
		cout<<"line "<<tokens[0]<<" : "<<" invalid syntax of the instruction :\n";
		error=true;
		return ;
	     }
	     for(int i=3;i<n;i+=2)
	     {
	        unsigned short ch=tokens[i];
		if(ch>255)
                {
                   cout<<"line "<<tokens[0]<<" : "<<" overflow!! value cannot fit within a byte : \""<<tokens[i]<<"\"\n";
                   error=true;
                   return;
                }
                char byte=ch;
	         out.write((char*)&byte,1);       
	     }             
        }
  }
  out.flush();
}


void generateMachineCode(String *lines, int numberOfLines, lineTokens *LT,optab &opcodeTab, symtab & symbolTab, char *fileName,bool &error)
{
    instruction inst=instruction();
    ofstream out(strcat(fileName,".pkrobj"),ios::binary);
    int opcodeIndex=0;
    unsigned int lastExecutableInstructionAddress=0;
    int i=0;

     //this while is just to get to the instruction where the memory related instruction starts and then assign the previous 
     //instruction address as the last instruction that can be executed
      while (lines[i].indexOf((char*)"byte") ==-1 && lines[i].indexOf((char*)"word") == -1 &&
	lines[i].indexOf((char*)"resb") == -1 && lines[i].indexOf((char*)"resw") == -1 && i < numberOfLines)
            i++; //increment till any one of the memory related instructions are encountered

      
      do {
              i--; //last executable instruction would be one instruction previous to the first memory related instruction
      } while(emptyLine(lines[i]));//skip all the empty lines till valid instruction
      
     
      lastExecutableInstructionAddress=lines[i].instructionAddress;
      out.write((char*)&lastExecutableInstructionAddress,4);       
      for(int i=0;i<numberOfLines;i++)
      {
          
          int n=0;
          unsigned char opcode=-1; // indicating no opcode initially
          unsigned short address=0;
          
           if(! emptyLine(lines[i])) 
           {
               String *tokens=LT[i].tokens;
               n=LT[i].n;
 
	       if( opcodeTab.find(tokens[1]) ) //if first token itself is the opcode then
	       {
		  opcodeIndex=1;
	       }
	       else
	       if(opcodeTab.find(tokens[2])) // if opcode is the second token then
	       {
		  opcodeIndex=2;
	       }
	      
		if( tokens[opcodeIndex] == (char*) "start" || tokens[opcodeIndex] == (char*) "end" || tokens[opcodeIndex] == (char*) "rsub")
		   continue;
		else // other than assembler directives (excluding rsub)
		{

	       
		if(tokens[opcodeIndex]==(char*)"resb" || tokens[opcodeIndex]==(char*)"resw" || tokens[opcodeIndex]==(char*)"byte" || tokens[opcodeIndex]==(char*)"word")
		   {
		      generateMachineCodeForMemoryInstructions(tokens,n,out,error);
		      continue;
		   }

		   opcode=getOpcode(tokens[opcodeIndex]);
		      
		   if( tokens[opcodeIndex] == (char*) "read" || tokens[opcodeIndex] == (char*) "disp")
		   {
			inst.setInstruction(opcode,0);
			out.write((char*)&inst,3);
			
		       continue;
		   }    
		   
		   if(! isRegisterInstruction(tokens[opcodeIndex]))
		   {
		       
		       address=symbolTab.getLabelAddress(tokens[opcodeIndex+1]);  
		       if(address==0)
		       {
			  error=true;
			  cout<<"line "<<tokens[0]<<" : "<<"undeclared variable \""<<tokens[opcodeIndex+1]<<"\"\n";		              
			  continue; //goto the next instruction
		       }

		       if( tokens[n-1].str[0]=='x' && tokens[n-1].getLength()==1 /* chechk for add, lda,sta,ldx,stx,etc*/ )
		       {
			       address+=32767;
			       inst.setInstruction(opcode, address);
			       out.write((char*)&inst,3);
			       cout<<lines[i]<<"\t\t\t\t"<<(int)inst.opcode<<"\t"<<inst.operand()<<endl;  			           
			       continue;
		       }
		       
		       
		       if( (opcodeIndex==1 && n>3) || (opcodeIndex==2 && n>4)  )//check for unwanted things after the instruction
		       {
			   error=true;
			   cout<<"line "<<tokens[0]<<" : "<< "unwanted character after the instructions\n";
			   continue;  //goto the next instruction
		       }
		      
		       inst.setInstruction(opcode, address);
		       out.write((char*)&inst,3);
		       cout<<lines[i]<<"\t\t\t\t"<<(int)inst.opcode<<"\t"<<inst.operand()<<endl;
		   }
		   else // register instruction
		   {
		      if(tokens[opcodeIndex]==(char*)"clear" || tokens[opcodeIndex]==(char*)"tixr" )//opcodes involving only one registers
		      {
			   if( (opcodeIndex==1 && n<3) || (opcodeIndex==2 && n<3) )// check whether is register is specified or not
			   {
			      error=true;
			      cout<<"line "<<tokens[0]<<" : "<< "No Register Specified\n";
				   continue;  //goto the next instruction		                  
			   }
			   
			   if(  (opcodeIndex==1 && n>3) || (opcodeIndex==2 && n>4) )  //check for unwanted things after the instruction
			   {
				   error=true;
				   cout<<"line "<<tokens[0]<<" : "<< "unwanted character after the instructions\n";
				   continue;  //goto the next instruction		                  
			   }
			   opcode=getOpcode(tokens[opcodeIndex]);
			   address=getRegisterNumber(tokens[opcodeIndex+1]);
			   if(address==10)
			   {
				   error=true;
				   cout<<"line "<<tokens[0]<<" : "<< "Invalid register: \""<<tokens[opcodeIndex+1]<<"\"\n";
				   continue;  //goto the next instruction		
			   }
			   inst.setInstruction(opcode,address);
			   out.write((char*)&inst,3);
			   cout<<lines[i]<<"\t\t\t\t"<<(int)inst.opcode<<"\t"<<inst.operand()<<endl;
		      }
		      else // register instructions with two registers involved
		      {
			    if(  (opcodeIndex==1 && n<5) || (opcodeIndex==2 && n<6) )//check whether both the registers are specified or not
			    {
				  error=true;
				  cout<<"line "<<tokens[0]<<" : "<< "Missing register(s)\n";
				  continue;  //goto the next instruction
			    }
			 
			   if(  (opcodeIndex==1 && n>5) || (opcodeIndex==2 && n>6) )  //check for unwanted things after the instruction
			   {
				   error=true;
				   cout<<"line "<<tokens[0]<<" : "<< "Invalid instruction: \""<<tokens[opcodeIndex+1]<<"\"\n";
				   continue;  //goto the next instruction		                  
			   }
			   opcode=getOpcode(tokens[opcodeIndex]);
			   address=getRegisterNumber(tokens[opcodeIndex+1]);//first register;
			   if(address==10)
			   {
				   error=true;
				   cout<<"line "<<tokens[0]<<" : "<< "Invalid register: \""<<tokens[opcodeIndex+1]<<"\"\n";
				   continue;  //goto the next instruction		
			   }
			   address*=256;                                    //shift by 8 bit to make space for next register info
			   if(getRegisterNumber(tokens[opcodeIndex+3])==10)
			   {
				   error=true;
				   cout<<"line "<<tokens[0]<<" : "<< "Invalid register: \""<<tokens[opcodeIndex+1]<<"\"\n";
				   continue;  //goto the next instruction		
			   }
			   address+=getRegisterNumber(tokens[opcodeIndex+3]);//second register excluding ',' comma
			   inst.setInstruction(opcode,address);
			   out.write((char*)&inst,3);
			   cout<<lines[i]<<"\t\t\t\t"<<(int)inst.opcode<<"\t"<<inst.operand()<<endl;
		      }
		   }
		}
           }
      }    
  if(error==true)
  {
     char s[100];
     s[0]='r'; s[1]='m'; s[2]=' ';
     strcat(s, fileName);     //if error(s) are found then the object flie created has to be deleted.
     system(s);
      exit(1);
  }

  out.close();
}












