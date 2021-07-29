#include<stdio.h>

enum Format_R{
sll,srl=0x02,jr=0x08,add=0x20,addu,sub,subu,and,or,nor=0x27,slt=0x2a,sltu};
enum Format_I{
beq=0x4,bne,addi=0x8,addiu,slti=0xa,sltiu,andi,ori,lui=0xf,lw=0x23,lbu,lhu,sb=0x28,sh,ll,sw=0x2b,sc=0x38};
enum Format_J{
j=0x2,jal};

int pc=0;

void raise_pc(int step);

int main (void){
 int SEI; int ZEI; int BA; int JA;
int op ;
int rs ;
int rt ;
int rd ;
int imm ;
int shamt;
int func;
int memory[0x2000]={0,};
int Reg[32]={0,};
int clock_cycle=0;
FILE *fd;
fopen_s(&fd,"fib.bin","rb");
while(!feof(fd)){
int a;

a=fread(memory+pc,4,1,fd);
   memory[pc]=(*(memory+pc)&0xFF000000)>>24|(*(memory+pc)&0xFF0000)>>8|(*(memory+pc)&0xFF00)<<8|(*(memory+pc)&0xFF)<<24;
    pc++;
}

pc=0;
Reg[31]=-1;
Reg[29]=0x8000;


while(pc != -1){
 
 clock_cycle++;
 printf("Now clock cycle : %d\n",clock_cycle);

op = (memory[pc/4]&0xFC000000)>>26;
rs = (memory[pc/4]&0x03E00000)>>21;
rt = (memory[pc/4]&0x001F0000)>>16;
rd = (memory[pc/4]&0x0000F800)>>11;
imm =(memory[pc/4]&0x0000FFFF);
shamt=(memory[pc/4]&0x000007C0)>>6;
func = (memory[pc/4]&0x0000003F);

if(imm >= 0x8000)
 {SEI = (short)imm;}
else
{SEI=imm;}

ZEI=imm;

BA=SEI<<2;

JA=(memory[pc/4+1]&0xF0000000)|((memory[pc/4]&0x03FFFFFF)<<2);


 
if(op != 0x0){
 
   if(op==j || op==jal){
    if(op==j){
  printf("Now memory[%d] is %x   -->   j\n",pc/4,memory[pc/4]);
     pc=JA;
  
  goto p;
 
}
   else if(op==jal){
	   printf("Now memory[%d] is %x   -->   jal\n",pc/4,memory[pc/4]);
      Reg[31]=pc+8; pc=JA;
	 
   goto p;
   }
   }
   
else{

if (op==addi)
{printf("Now memory[%d] is %x   -->   addi\n",pc/4,memory[pc/4]);
	Reg[rt]=Reg[rs]+SEI;
  raise_pc(4);
  goto p;
}

else if(op==addiu)
{ 
 Reg[rt]=Reg[rs]+SEI;
    printf("Now memory[%d] is %x   -->   addiu\n",pc/4,memory[pc/4]);
   raise_pc(4);
   
   goto p;
   
}
else if(op==andi)
{
  Reg[rt]=Reg[rs]&ZEI;
   printf("Now memory[%d] is %x   -->   andi\n",pc/4,memory[pc/4]);
   raise_pc(4);
   goto p;
}

else if(op==beq)

{ printf("Now memory[%d] is %x   -->   beq\n",pc/4,memory[pc/4]);
	if(Reg[rt]==Reg[rs])
pc=pc+4+BA;
else 
 raise_pc(4);

goto p;
}

else if(op==bne)
{ printf("Now memory[%d] is %x   -->   bne\n",pc/4,memory[pc/4]);
{if(Reg[rs] != Reg[rt])
pc=pc+4+BA;
else 
 raise_pc(4);

goto p;
}
}
else if(op==lbu)
{ printf("Now memory[%d] is %x   -->   lbu\n",pc/4,memory[pc/4]);
	Reg[rt]=memory[(Reg[rs]+SEI)/4]&0xFF;
   raise_pc(4);
   goto p;
}
else if(op==lhu)
{ printf("Now memory[%d] is %x   -->   lhu\n",pc/4,memory[pc/4]);
	Reg[rt]=memory[(Reg[rs]+SEI)/4]&0xFFFF;
   raise_pc(4);
   goto p;
}
else if(op==ll)
{printf("Now memory[%d] is %x   -->   ll\n",pc/4,memory[pc/4]);
	Reg[rt]=memory[(Reg[rs]+SEI)/4];
   raise_pc(4);
   goto p;
}
else if(op==lui)
{ printf("Now memory[%d] is %x   -->   lui\n",pc/4,memory[pc/4]);
	Reg[rt]=imm>>16;
    raise_pc(4);
 goto p;
}
else if(op==lw)
{ printf("Now memory[%d] is %x   -->   lw\n",pc/4,memory[pc/4]);
	Reg[rt]=memory[(Reg[rs]+SEI)/4];
    raise_pc(4);
 goto p;
}
else if(op==ori)
{printf("Now memory[%d] is %x   -->   ori\n",pc/4,memory[pc/4]);
	Reg[rt]=Reg[rs] | ZEI;
   raise_pc(4);
   goto p;
}
else if(op==slti)
{ printf("Now memory[%d] is %x   -->   slti\n",pc/4,memory[pc/4]);
 if(Reg[rs]<SEI)
    Reg[rt]=0x1;
else
    Reg[rt]=0x0;
   raise_pc(4);
   goto p;
}
else if(op==sltiu)
{  printf("Now memory[%d] is %x   -->   sltiu\n",pc/4,memory[pc/4]);
	if(Reg[rs]<SEI)
    Reg[rt]=0x1;
else
    Reg[rt]=0x0;

   raise_pc(4);
   goto p;
}
else if(op==sb)
{ printf("Now memory[%d] is %x   -->   sb\n",pc/4,memory[pc/4]);
	memory[(Reg[rs]+SEI)/4]=Reg[rt]&0xFF;
 raise_pc(4);
 goto p;
}
/*
else if(op==sc)
{memory[Reg[rs]+SEI]=Reg[rt];
Reg[rt](atomic)?1:0}
*/
else if(op==sh)
{ printf("Now memory[%d] is %x   -->   sh\n",pc/4,memory[pc/4]);
	memory[(Reg[rs]+SEI)/4]=Reg[rt]&0xFFFF;
   raise_pc(4);
   goto p;
}
else if(op==sw)
{ printf("Now memory[%d] is %x   -->   sw\n",pc/4,memory[pc/4]);
 memory[(Reg[rs]+SEI)/4]=Reg[rt];
 raise_pc(4);
 goto p;
}
}
}

 

 

 

 


else
{
if(func==add)
{ printf("Now memory[%d] is 0%x   -->   add\n",pc/4,memory[pc/4]);
	Reg[rd]=Reg[rs]+Reg[rt];
   raise_pc(4);
   goto p;
}
else if(func==addu)
{ printf("Now memory[%d] is 0%x   -->   addu\n",pc/4,memory[pc/4]);
 Reg[rd]=Reg[rs]+Reg[rt];
   raise_pc(4);
   goto p;
}
else if(func==and)
{ printf("Now memory[%d] is 0%x   -->   and\n",pc/4,memory[pc/4]);
	Reg[rd]=Reg[rs]&Reg[rt];
   raise_pc(4);
   goto p;
}
else if(func==jr)
{ printf("Now memory[%d] is 0%x   -->   jr\n",pc/4,memory[pc/4]);
	pc=Reg[rs];//Reg[rs];
   goto p;
}
else if(func==nor)
{printf("Now memory[%d] is 0%x   -->   nor\n",pc/4,memory[pc/4]);
	Reg[rd]=~(Reg[rs]|Reg[rt]);
   raise_pc(4);
   goto p;
}
else if(func==or)
{printf("Now memory[%d] is 0%x   -->   or\n",pc/4,memory[pc/4]);
	Reg[rd]=Reg[rs]|Reg[rt];
   raise_pc(4);
   goto p;
}
else if(func==slt)
{printf("Now memory[%d] is 0%x   -->   slt\n",pc/4,memory[pc/4]);
	if(Reg[rs]<Reg[rt])
    Reg[rd]=0x1;
else
    Reg[rd]=0x0;

   raise_pc(4);
   goto p;
}
else if(func==sltu)
{  printf("Now memory[%d] is 0%x   -->   sltu\n",pc/4,memory[pc/4]);
	if(Reg[rs]<Reg[rt])
      Reg[rd]=0x1;
  else
      Reg[rd]=0x0;

   raise_pc(4);
   goto p;
}
else if(func==sll)
{printf("Now memory[%d] is 0%x   -->   sll\n",pc/4,memory[pc/4]);
	Reg[rd]=Reg[rt];
   raise_pc(4);
   goto p;
}
else if(func==srl)
{printf("Now memory[%d] is 0%x   -->   srl\n",pc/4,memory[pc/4]);
	Reg[rd]=Reg[rt];
   raise_pc(4);
   goto p;
}
else if(func==sub)
{printf("Now memory[%d] is 0%x   -->   sub\n",pc/4,memory[pc/4]);
	Reg[rd]=Reg[rs]-Reg[rt];
   raise_pc(4);
   goto p;
}
else if(func==subu)
{printf("Now memory[%d] is 0%x   -->   subu\n",pc/4,memory[pc/4]);
	Reg[rd]=Reg[rs]-Reg[rt];
   raise_pc(4);
   goto p;
}
}

p:printf("The pc of clock cycle %d  = %d\n",clock_cycle,pc);
printf("value of Register[2]=%d\n\n",Reg[2]);

}
printf("The final value is %d!!!!!\n", Reg[2]);

}

void raise_pc(int step)
{ 
 pc+= step;
}
