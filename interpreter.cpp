#include "processor.cpp"


int main(int argc, char ** argv)
{
     if(argc<2)
     {
        cout<<"Improper Arguments!";
        return 1;
     }
   /*instruction inst=instruction();
   ifstream in(argv[1],ios::binary);
   int fileSize=getSize(in);
   int temp;
   in.read((char*)&temp,4);
   while(fileSize>2)
   {
      in.read((char*)&inst,3);
      cout<<(int)inst.opcode<<"\t"<<inst.operand()<<endl;
      fileSize-=3;
   }*/

  processor p=processor(argv[1]);
  p.execute(); 
}

