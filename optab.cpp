#include "string.cpp"


int getSize(ifstream &in)
{
   int fileSize=0;
   in.seekg(0,ios::end);
   fileSize=in.tellg();
   in.seekg(0,ios::beg);
   return fileSize;
}

unsigned char getOpcode(String opcode)
{
   if(opcode==(char*)"add") //done
      return 0x18;

   if(opcode==(char*)"addr")//done
      return 0x90;
      
   if(opcode==(char*)"and") 
      return 0x40;

   if(opcode==(char*)"clear") //done
      return 0xB4;

   if(opcode==(char*)"comp") //done
      return 0x28;

   if(opcode==(char*)"compr") //done
      return 0xA0;

   if(opcode==(char*)"div")
      return 0x24;

   if(opcode==(char*)"divr") //done
      return 0x9C;

   if(opcode==(char*)"jeq") //done
      return 0x30;
      
   if(opcode==(char*)"j") //done
      return 0x3C;

   if(opcode==(char*)"jgt")//done
      return 0x34;

   if(opcode==(char*)"jlt") //done
      return 0x38;

   if(opcode==(char*)"jsub")
      return 0x48;

   if(opcode==(char*)"lda") // done
      return 0x00;

   if(opcode==(char*)"ldb") //done
      return 0x68;

   if(opcode==(char*)"ldch") //done
      return 0x50;

   if(opcode==(char*)"ldl") //done
      return 
      0x08;

   if(opcode==(char*)"lds") // done
      return 0x6C;

   if(opcode==(char*)"ldt") //done
      return 0x74;

   if(opcode==(char*)"ldx") //done
      return 0x04;

   if(opcode==(char*)"mul") 
      return 0x20;

   if(opcode==(char*)"mulr") //done
      return 0x98;

   if(opcode==(char*)"or")
      return 0x44;

   if(opcode==(char*)"rd")
      return 0xD8;

   if(opcode==(char*)"rmo") //done
      return 0xAC;

   if(opcode==(char*)"rsub")
      return 0x4C;

   if(opcode==(char*)"sta")//done
      return 0x0C;

   if(opcode==(char*)"stb")//done
      return 0x78;

   if(opcode==(char*)"stch")//done
      return 0x54;

   if(opcode==(char*)"stl") // done
      return 0x14;

   if(opcode==(char*)"sts") //done
      return 0x7C;

   if(opcode==(char*)"stt") //done
      return 0x84;

   if(opcode==(char*)"stx") //done
      return 0x10;

   if(opcode==(char*)"sub")
      return 0x1C;

   if(opcode==(char*)"subr") //done
      return 0x94;

   if(opcode==(char*)"td")
      return 0xE0;

   if(opcode==(char*)"tio")
      return 0xF8;
      
   if(opcode==(char*)"tix") //done
      return 0x2C;

   if(opcode==(char*)"tixr") //done
      return 0xB8;

   if(opcode==(char*)"wd")
      return 0xDC;

   if(opcode==(char*)"read") //done
      return 0xEC;

  if(opcode==(char*)"disp") //done
      return 0xED;

  if(opcode==(char*)"strd") //done
      return 0xEE;
      
  if(opcode==(char*)"reads") //done
      return 0xEF;
}

int getRegisterNumber(String s)
{
    if(s==(char*)"a") 
      return 0;

    if(s==(char*)"x") 
      return 1;

    if(s==(char*)"l") 
      return 2;

    if(s==(char*)"b") 
      return 3;

    if(s==(char*)"s") 
      return 4;

    if(s==(char*)"t") 
      return 5;

    if(s==(char*)"pc") 
      return 8;
     
    if(s==(char*)"sw") 
      return 9;

  return 10;   // not a register 
}


class optab
{
   public:
   String *opcodes;
   int numberOfOpcodes;
   optab()
   {
       ifstream in("optab",ios::binary);
       int fileSize=getSize(in);
       String temp(fileSize);
       in.read((char*)temp.str,fileSize);
       opcodes=temp.tokenize(' ',numberOfOpcodes);        
   }

    int find(String s)
    {
       for(int i=0;i<numberOfOpcodes;i++)
       {
           if(opcodes[i]==s)
                return 1;
       }
       return 0;     
    }

    void display()
    {
       for(int i=0;i<numberOfOpcodes;i++)
         cout<<opcodes[i]<<" ";
    }
};
