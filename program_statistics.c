
#include <stdio.h>
#include <stdlib.h>

#define MAX_HEX_VALUES 10000

unsigned int hex_values[MAX_HEX_VALUES];
int registerRW[32][2];

int numDynamic = 0;
int numHexValues = 0;

int numR = 0;
int numJ = 0;
int numI = 0;

int numForwardTaken = 0;
int numBackTaken = 0;
int numNonTaken = 0;
float percentageForward = 0;
float percentageBackward = 0;
float percentageNonTaken = 0;

int numArithmetic = 0;
int numLoads = 0;
int numStores = 0;
float percentLoads = 0;
float percentStores = 0;
float percentArith = 0;

unsigned int pcBefore = 0;
unsigned int pcAfter = 0;



void getStats(unsigned int instruction, unsigned int index){
    char type = 'A';
    int opcode = instruction >> 26;

    if (opcode == 0) {
      type = 'R';
      numR++;
    } else if (opcode == 2 || opcode == 3) {
      type = 'J';
      numJ++;
    } else {
      type = 'I';
      numI++;
    }


    if(type == 'J'){
      pcBefore = hex_values[index - 1];
      pcAfter = hex_values[index + 1];

      if(pcAfter > pcBefore){
        numForwardTaken++;
      }else{
        numBackTaken++;
      }
    }


    if(type == 'R'){
      unsigned int rs;
      unsigned int rt;
      unsigned int rd;
      unsigned int shamt;
      unsigned int funct;

      unsigned int mask = ~(0xFFFFFFFF << 26);
      unsigned int temp = instruction;
      unsigned int r = instruction & mask;
      rs = r >> 21;


      unsigned int mask2 = ~(0xFFFFFFFF << 21);
      unsigned int r1 = instruction & mask2;
      rt = r1 >> 16;

      unsigned int mask3 = ~(0xFFFFFFFF << 16);
      unsigned int r2 = instruction & mask3;
      rd = r2 >> 11;


      unsigned int mask4 = ~(0xFFFFFFFF << 11);
      unsigned int r3 = instruction & mask4;
      shamt = r3 >> 6;


      unsigned int mask5 = ~(0xFFFFFFFF << 6);
      funct = instruction & mask5;

      switch(funct){
        case 32: //add
          numArithmetic++;
          break;
        case 33: //addu
          numArithmetic++;
          break;
        case 36: //and
          numArithmetic++;
          break;
        case 39: //nor
          numArithmetic++;
          break;
        case 37: //or
          numArithmetic++;
          break;
        case 42: //slt
          numArithmetic++;
          break;
        case 43: //sltu
          numArithmetic++;
          break;
        case 0: //sll
          numArithmetic++;
          break;
        case 3: //sra
          numArithmetic++;
          break;
        case 2: //srl
          numArithmetic++;
          break;
        case 34: //sub
          numArithmetic++;
          break;
        case 38: //xor
          numArithmetic++;
          break;
        case 8: //jr
        pcBefore = hex_values[index - 1];
        pcAfter = hex_values[index + 1];
        if(pcBefore > pcAfter){
          numBackTaken++;
        }else{
          numForwardTaken++;
        }
        break;
      }
      if(rs > 0){
        registerRW[rs][0]++;
      }
      registerRW[rd][1]++;
      if(rt > 0){
        registerRW[rt][0]++;
      }


    }


    if(type =='I'){

      unsigned int rs;
      unsigned int rt;
      unsigned int imm;

      unsigned int mask = ~(0xFFFFFFFF << 26);
      unsigned int temp = instruction;
      unsigned int r = instruction & mask;
      rs = r >> 21;


      unsigned int mask2 = ~(0xFFFFFFFF << 21);
      unsigned int r1 = instruction & mask2;
      rt = r1 >> 16;


      unsigned int mask3 = ~(0xFFFFFFFF << 16);
      imm = instruction & mask3;

      switch(opcode){
        case 8: //addi
          numArithmetic++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 9: //addiu
          numArithmetic++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 12: //andi
          numArithmetic++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 15: //lui
          numLoads++;
          registerRW[rt][1]++;//Increments Wrties
          break;
        case 13: //ori
          numArithmetic++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 10: //slti
          numArithmetic++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 11: //sltiu
          numArithmetic++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 36: //lbu
          numLoads++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 37://or
          numLoads++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 35: //lw
          numLoads++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 40: //sb
          numStores++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 41: //sh
          numStores++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 43: //sw
          numStores++;
          registerRW[rt][1]++;//Increments Wrties
          registerRW[rs][0]++;//Increments Reads
          break;
        case 4: //beq
          pcBefore = hex_values[index - 1];
          pcAfter = hex_values[index + 1];
          if(pcAfter == pcBefore + 4){
            numNonTaken++;
          }else if(pcBefore > pcAfter){
            numBackTaken++;
          }else{
            numForwardTaken++;
          }
          registerRW[rt][0]++;//Increments Reads
          registerRW[rs][0]++;//Increments Reads
          break;
        case 5: //bne
          pcBefore = hex_values[index - 1];
          pcAfter = hex_values[index + 1];
          if(pcAfter == pcBefore + 4){
            numNonTaken++;
          }else if(pcBefore > pcAfter){
            numBackTaken++;
          }else{
            numForwardTaken++;
          }
          registerRW[rt][0]++;//Increments Reads
          registerRW[rs][0]++;//Increments Reads
          break;
        }
    }
}


void initReadWriteArray(){
  for (int r = 0; r < 32; r++){
    for (int c = 0; c < 2; c ++){
      registerRW[r][c] = 0;
    }
  }
}


void readFile(){
  FILE *file;
  char fileName[] = "trace.txt";
  char hex_string[9];
  char *endptr;

  file = fopen(fileName, "r");
   if (file == NULL) {
     perror("Error opening file");
   }

  while (fscanf(file, "%8s", hex_string) != EOF && numHexValues < MAX_HEX_VALUES) {
    hex_values[numHexValues] = strtoul(hex_string, &endptr, 16);
    numHexValues++;
  }

  fclose(file);
}


void calculatePercentages(){
  numDynamic = numHexValues / 2;
  percentageForward = ((float)numForwardTaken / (float)numDynamic) * 100;
  percentageBackward = ((float)numBackTaken / (float)numDynamic) * 100;
  percentageNonTaken = ((float)numNonTaken / (float)numDynamic) * 100;
  percentLoads = ((float)numLoads / (float)numDynamic) * 100;
  percentStores = ((float)numStores / (float)numDynamic) * 100;
  percentArith = ((float)numArithmetic / (float)numDynamic) * 100;
}


void printStats(){
  printf("insts: %d\n", numDynamic);
  printf("r-type: %d\n", numR);
  printf("i-type %d\n", numI);
  printf("j-type %d\n", numJ);
  printf("fwd-taken: %f\n", percentageForward);
  printf("bkw-taken: %f\n", percentageBackward);
  printf("not-taken: %f\n", percentageNonTaken);
  printf("loads: %f\n", percentLoads);
  printf("stores: %f\n", percentStores);
  printf("arith: %f\n", percentArith);
  for(int i = 0; i < 32; i++){
    printf("reg -%d: %d %d\n", i, registerRW[i][0], registerRW[i][1]);
  }
}



int main() {
  initReadWriteArray();
  readFile();
  for(int i = 1; i < numHexValues; i+=2){
    getStats(hex_values[i], i);
  }
  calculatePercentages();
  printStats();
}
