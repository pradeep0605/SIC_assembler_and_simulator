#include "symtab.cpp"	
#include<stdio.h>
using namespace std;

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

class conditionCode
{
   public:
   bool  lessThan, equal, greaterThan;
   conditionCode()
   {
      lessThan=equal=greaterThan=false;
   }   
};

class processor
{
   int regBank[9];
   conditionCode cc;
   ifstream in;
   char *codeAndData;
   char *inputCharBuffer;
   int addressOfLastExeInst; // both instruction and data
   public:
        processor(const processor &p)
        {
           
        }
        processor(char *s)
	{ 
	   inputCharBuffer=new char[256];
	   inputCharBuffer[0]='\0';
	   in.open(s,ios::binary);
	   for(int i=0;i<9;i++)
	      regBank[i]=0;

	   int fileSize=getSize(in); // get The file Size
	   codeAndData=new char[fileSize-4 + 1];//allocate memory for the entire program (-4 as first 4 bytes tell addr of last exe instruction)
	   addressOfLastExeInst=0;           
	   in.read((char*)&addressOfLastExeInst,4);//read the first 4 bytes which gives the address of the last executable instruction
	   in.read((char*)codeAndData,fileSize-4); //read the entire program and store it in codeAndData
	}
   
   void executeAll(char *);   
   void runInstruction(instruction *inst, char *);
   void execute()
   {
       executeAll(codeAndData);
   }
};


void processor :: executeAll(char *codeAndData)
{
    instruction *inst;
    regBank[8]=0; //set program counter to zero   
    inst=(instruction*)&(codeAndData[regBank[8]]);
    while((int)inst<= (int)(instruction*)codeAndData + addressOfLastExeInst)
    {
         //cout<<"INST:"<<(int)inst<<"    code & data + AOLI :"<<(int)(instruction*)codeAndData+addressOfLastExeInst<<endl;
         inst=(instruction*)&(codeAndData[regBank[8]]);
         regBank[8]+=3;
         runInstruction(inst,codeAndData);
         
    }

}

void processor :: runInstruction(instruction *inst,char *codeAndData)
{
          /*cout<<"Opcode"<<int(inst->opcode)<<endl;
            cout.flush();*/
   int *temp, intrm=0,k=0;
   char ch=0;
   char *charr;
   switch(inst->opcode)
   {    
  
       case 0xB4: //clears the specified register
                  regBank[int(inst->op2)]=0;
                   break;

                   
       case 0xEC:  //read from the console
                
                   if(regBank[4]==0)// read a word from the console
                   {
                      int word;
                      cin>>word;
                      if(word>16777215)
                      {
                        cout<<"terminated abnormally!! Over flow \n";
                        cout.flush();
                        exit(0);
                      }  
                      regBank[0]=word;
                   }
                   if(regBank[4]==1) // read a integer that fints into a byte
                   { 
                      int word;
                      cin>>word;
                      if(word>255)
                      {
                       cout<<"terminated abnormally!! Over flow \n";
                       cout.flush();
                        exit(0);
                      }
                      regBank[0]=word;
                      break;
                   }
                   
                   break;
                   
      case 0xEF:       // reads a string until newline and places it in the specified memroy location.
                       k=0;
                       
                       if(inst->operand() >=32767)//if indexed
                       {
                          while((ch=getchar())!='\n')
                              codeAndData[ inst->operand() + regBank[1] - 32767 + k++ ]=ch; // if indexed
        
                             codeAndData[inst->operand() + regBank[1] - 32767 + k]='\0';
                       }
   	               else
                       {
                          while((ch=getchar())!='\n')
                              codeAndData[ inst->operand() + k++]=ch;
                     
                            codeAndData[inst->operand() + k]='\0';
                       }
                       break;
                       
                       
                       
       case 0xED: // displays a string or a byte or a word
		   if(regBank[4]==0)// displays a word from the console
                   {  
                      cout<<regBank[0];
                      cout.flush();
                                                                
                   }
                   if(regBank[4]==1) // displays a integer that fints into a byte
                   {
                       cout<<regBank[0];
		       cout.flush();
                   }
                   if(regBank[4]==2) // displays a character at a time
                   {
                       cout<<char(regBank[0]);
                       cout.flush();
                   }
			         break;

			         
       case 0xEE: //displays a string specified in the instruction
                   if(inst->operand() >=32767)
                        cout<< &(codeAndData[inst->operand() + regBank[1]  - 32767]  );
   	           else
      	                cout<< &(codeAndData[inst->operand()]);
      	                
      	                cout.flush();
   	           break;
                   
       case 0x00: //loads a word from specified memory location to register a 
                   if(inst->operand() >=32767)
                      temp= (int *) &(codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                              // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      temp= (int *) &(codeAndData[inst-> operand() ]);

       		   regBank[0]=*temp;  //0 means register a
       		   regBank[0]=regBank[0]<<8; // as 4 bytes are read !! JUST HAVE TO CLEAR THE HIGHER 8 BITS
       		   regBank[0]=regBank[0]>>8;
                   break;

       case 0x6C: //loads a word from specified memory location to register s
                   if(inst->operand() >=32767)
                      temp= (int *) &(codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                              // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      temp= (int *) &(codeAndData[inst-> operand() ]);
       		   regBank[4]=*temp; // 4 means register s
		   regBank[4]=regBank[4]<<8; // as 4 bytes are read !! JUST HAVE TO CLEAR THE HIGHER 8 BITS
       		   regBank[4]=regBank[4]>>8;
                   
                   break;

       case 0x68: //loads a word from specified memory location to register b
                  if(inst->operand() >=32767)
                      temp= (int *) &(codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                              // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      temp= (int *) &(codeAndData[inst-> operand() ]);
       		   regBank[3]=*temp; // 3 means register b
		   regBank[3]=regBank[3]<<8; // as 4 bytes are read !! JUST HAVE TO CLEAR THE HIGHER 8 BITS
       		   regBank[3]=regBank[3]>>8;

                   break;

       case 0x74: //loads a word from specified memory location to register t
                   if(inst->operand() >=32767)
                      temp= (int *) &(codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                              // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      temp= (int *) &(codeAndData[inst-> operand() ]);
       		   regBank[5]=*temp; // 5 means register t 
		   regBank[5]=regBank[5]<<8; // as 4 bytes are read !! JUST HAVE TO CLEAR THE HIGHER 8 BITS
       		   regBank[5]=regBank[5]>>8;

                   break;
                   

      case 0x04: //loads a word from specified memory location to register x
                   if(inst->operand() >=32767)
                      temp= (int *) &(codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                              // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      temp= (int *) &(codeAndData[inst-> operand() ]);
       		   regBank[1]=*temp; // 1 means register x
		   regBank[1]=regBank[1]<<8; // as 4 bytes are read !! JUST HAVE TO CLEAR THE HIGHER 8 BITS
       		   regBank[1]=regBank[1]>>8;

                   break;

       case 0x08: //loads a word from specified memory location to register l
                   if(inst->operand() >=32767)
                      temp= (int *) &(codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                               // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      temp= (int *) &(codeAndData[inst-> operand() ]);
       		   regBank[2]=*temp; // 1 means register x
		   regBank[2]=regBank[2]<<8; // as 4 bytes are read !! JUST HAVE TO CLEAR THE HIGHER 8 BITS
       		   regBank[2]=regBank[2]>>8;

                   break;

       case 0x50: //loads a character into register 'a'
       
                   if(inst->operand() >=32767)
                      ch= (codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                               // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      ch=(codeAndData[inst-> operand() ]);
       		   regBank[0]=ch; // 0 means register a

                   break;

       case 0x54: //stores a character to a specified memory locations.
       
                   if(inst->operand() >=32767)
                    codeAndData[inst-> operand() + regBank[1] - 32767 ]= char(regBank[0]); // - 32768 because it will be added in the
                                               // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                    codeAndData[inst-> operand() ] = char(regBank[0]);
                   break;regBank[8]+=3;
                   
                   
       case 0x0C: //stores a word in the register 'a' to given specified location
       
                   if(inst->operand() >=32767)
                   {
                     charr=(char*)&(regBank[0]); // convert to char array TO assign three bytes (word)
                     codeAndData[inst-> operand() + regBank[1] - 32767 ]= charr[0]; //first byte is assigned to lowest address
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 1 ]= charr[1];//second byte is assigned to next lowest addresss
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 2 ]= charr[2];//third byte is assigned to the higest address.  
                   }                            // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                   {
                       charr=(char*)&(regBank[0]);
                       codeAndData[inst-> operand()  + 0 ]= charr[0]; 
                       codeAndData[inst-> operand()  + 1 ]= charr[1];
                       codeAndData[inst-> operand()  + 2 ]= charr[2];
                   }
                   break;
      
      case 0x10: //stores a word in the register 'x' to given specified location
       
                   if(inst->operand() >=32767)
                   {
                     charr=(char*)&(regBank[1]); // convert to char array TO assign three bytes (word)
                     codeAndData[inst-> operand() + regBank[1] - 32767 ]= charr[0]; //first byte is assigned to lowest address
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 1 ]= charr[1];//second byte is assigned to next lowest addresss
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 2 ]= charr[2];//third byte is assigned to the higest address.  
                   }                            // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                   {
                       charr=(char*)&(regBank[1]);
                       codeAndData[inst-> operand()  + 0 ]= charr[0]; 
                       codeAndData[inst-> operand()  + 1 ]= charr[1];
                       codeAndData[inst-> operand()  + 2 ]= charr[2];
                   }
                   break;
       
       
       case 0x14: //stores a word in the register 'l' to given specified location
       
                   if(inst->operand() >=32767)
                   {
                     charr=(char*)&(regBank[2]); // convert to char array TO assign three bytes (word)
                     codeAndData[inst-> operand() + regBank[1] - 32767 ]= charr[0]; //first byte is assigned to lowest address
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 1 ]= charr[1];//second byte is assigned to next lowest addresss
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 2 ]= charr[2];//third byte is assigned to the higest address.  
                   }                            // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                   {
                       charr=(char*)&(regBank[2]);
                       codeAndData[inst-> operand()  + 0 ]= charr[0]; 
                       codeAndData[inst-> operand()  + 1 ]= charr[1];
                       codeAndData[inst-> operand()  + 2 ]= charr[2];
                   }
                   break;
                   
       
       case 0x78: //stores a word in the register 'b' to given specified location
       
                   if(inst->operand() >=32767)
                   {
                     charr=(char*)&(regBank[3]); // convert to char array TO assign three bytes (word)
                     codeAndData[inst-> operand() + regBank[1] - 32767 ]= charr[0]; //first byte is assigned to lowest address
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 1 ]= charr[1];//second byte is assigned to next lowest addresss
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 2 ]= charr[2];//third byte is assigned to the higest address.  
                   }                            // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                   {
                       charr=(char*)&(regBank[3]);
                       codeAndData[inst-> operand()  + 0 ]= charr[0]; 
                       codeAndData[inst-> operand()  + 1 ]= charr[1];
                       codeAndData[inst-> operand()  + 2 ]= charr[2];
                   }
                   break;
                   
       case 0x7C: //stores a word in the register 's' to given specified location
       
                   if(inst->operand() >=32767)
                   {
                     charr=(char*)&(regBank[4]); // convert to char array TO assign three bytes (word)
                     codeAndData[inst-> operand() + regBank[1] - 32767 ]= charr[0]; //first byte is assigned to lowest address
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 1 ]= charr[1];//second byte is assigned to next lowest addresss
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 2 ]= charr[2];//third byte is assigned to the higest address.  
                   }                            // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                   {
                       charr=(char*)&(regBank[4]);
                       codeAndData[inst-> operand()  + 0 ]= charr[0]; 
                       codeAndData[inst-> operand()  + 1 ]= charr[1];
                       codeAndData[inst-> operand()  + 2 ]= charr[2];
                   }
                   break;
                   
       
       case 0x84: //stores a word in the register 't' to given specified location
       
                   if(inst->operand() >=32767)
                   {
                     charr=(char*)&(regBank[5]); // convert to char array TO assign three bytes (word)
                     codeAndData[inst-> operand() + regBank[1] - 32767 ]= charr[0]; //first byte is assigned to lowest address
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 1 ]= charr[1];//second byte is assigned to next lowest addresss
                     codeAndData[inst-> operand() + regBank[1] - 32767 + 2 ]= charr[2];//third byte is assigned to the higest address.  
                   }                            // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                   {
                       charr=(char*)&(regBank[5]);
                       codeAndData[inst-> operand()  + 0 ]= charr[0]; 
                       codeAndData[inst-> operand()  + 1 ]= charr[1];
                       codeAndData[inst-> operand()  + 2 ]= charr[2];
                   }
                   break;

      
      case 0x3C:   // unconditional jump to specified instruction
                   
      		   regBank[8]= inst->operand();
                   break;
      
      
      case 0x30:   // conditional jump to specified instruction if equal flag is set
                   if(cc.equal)
      		   regBank[8]= inst->operand();
                   
                   break;
      
      case 0x38:   // conditional jump to specified instruction if less than flag is set
                   if(cc.lessThan)
      		   regBank[8]= inst->operand();     //updating pc.
                   
                   break;
                   
      case 0x34:   // conditional jump to specified instruction if greater than flag is set
                   if(cc.greaterThan)
      		   regBank[8]= inst->operand();
      		   
                   break;
      
                       
      case 0x28:  //to compare the value at the specified memory location with register 'a'
                     if(inst->operand() >=32767)
                      temp= (int *) &(codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                               // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      temp= (int *) &(codeAndData[inst-> operand() ]);
                      
                   intrm=*temp;
                   intrm=intrm<<8;  // as integer is of 4 bytes here. the higher 4 th bytes should be cleared.
                   intrm=intrm>>8;
                   
                   if(intrm>regBank[0])
                   {
                      cc.lessThan=true;
                      cc.equal=cc.greaterThan=false;
                   }
                   if(intrm==regBank[0])
                   {
                     cc.equal=true;
                     cc.lessThan=cc.greaterThan=false;
                   }  
                   if(intrm<regBank[0])
                   {
                     cc.greaterThan=true;
                     cc.lessThan=cc.equal=false;
                   }
                   break;
                   
 case 0xA0:  //to compare the value of the two registers
 		                      
                   if(regBank[inst->op1] < regBank[inst->op2] )
                   {
                      cc.lessThan=true;
                      cc.equal=cc.greaterThan=false;
                   }
                   if(regBank[inst->op1] == regBank[inst->op2])
                   {
                     cc.equal=true;
                     cc.lessThan=cc.greaterThan=false;
                   }  
                   if(regBank[inst->op1] > regBank[inst->op2])
                   {
                     cc.greaterThan=true;
                     cc.lessThan=cc.equal=false;
                   }
                   break;        

      case 0x2C: //Increments the register 'x' by one and compares with a memory location
                   if(inst->operand() >=32767)
                      temp= (int *) &(codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                               // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      temp= (int *) &(codeAndData[inst-> operand() ]);
                   
                   regBank[1]=regBank[1] + 1;
                   if(regBank[1] < *temp)
                   {
                      cc.lessThan=true;
                      cc.equal=cc.greaterThan=false;
                   }
                   if(regBank[1] == *temp)
                   {
                     cc.equal=true;
                     cc.lessThan=cc.greaterThan=false;
                   }  
                   if(regBank[1] > *temp)
                   {
                     cc.greaterThan=true;
                     cc.lessThan=cc.equal=false;
                   } 
                   break;           
                   
     case 0xB8: //Increments the register 'x' by one and compares with a memory location
                                     
                   regBank[1]=regBank[1] + 1;
                   if(regBank[1] < regBank[inst->op1])
                   {
                      cc.lessThan=true;
                      cc.equal=cc.greaterThan=false;
                   }
                   if(regBank[1] == regBank[inst->op1])
                   {
                     cc.equal=true;
                     cc.lessThan=cc.greaterThan=false;
                   }  
                   if(regBank[1] > regBank[inst->op1])
                   {
                     cc.greaterThan=true;
                     cc.lessThan=cc.equal=false;
                   } 
                   break;           
                   
                   
      case 0x18: //Adds register 'a' with the specified memory location.
                   if(inst->operand() >=32767)
                      temp= (int *) &(codeAndData[inst-> operand() + regBank[1] - 32767 ]); // - 32768 because it will be added in the
                                               // assembling to indicate that its a indexed instruction hence we have to subtract 32768
                   else 		
                      temp= (int *) &(codeAndData[inst-> operand() ]);
                   
                   k=*temp; 
                   k=k<<8; 
                   k=k>>8; // The fourth byte is also read instead of 3, hence clearing first 8 bits.
                   regBank[0]=regBank[0] + k;
                   break;                   

      case 0xAC: // moves data between registers.
                   //cout<< regBank[ int(inst->op1)]<<"   "<< regBank[int(inst->op2)]<<endl;
       		   regBank[ int(inst->op2)]= regBank[ int(inst->op1) ];
                   break;

      case 0x90: //adds two registers and saves the result in the second register.
                   regBank[ int(inst->op2)]= regBank[ int(inst->op1) ] + regBank[ int(inst->op2) ];
                   break;


       case 0x94: //subtracts two registers and saves the result in the second register.
                   regBank[ int(inst->op2)]= regBank[ int(inst->op1) ] - regBank[ int(inst->op2) ];
                   break; 
                   
       case 0x98: //multiplies two registers and saves the result in the second register.
                   regBank[ int(inst->op2)]= regBank[ int(inst->op1) ] * regBank[ int(inst->op2) ];
                   break;       

       case 0x9C: //divides two registers and saves the result in the second register.
                   regBank[ int(inst->op2)]= regBank[ int(inst->op1) ] / regBank[ int(inst->op2) ];
                   regBank[2]= regBank[ int(inst->op1) ] % regBank[ int(inst->op2) ];
                   break;                   
                               

   }
      
}












