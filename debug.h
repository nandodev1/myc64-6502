#define CHARACTER  2
#define HEXADECIMAL  1
#define INTEGER  0


static char * get_flags();
const char * get_comand( uint8_t opc);
void display_ram(int bloco);
void display_ram_continuo(uint16_t adress_start, uint16_t adress_end, uint8_t quant_columm,uint8_t tipo_exibir);
void display_ram_for_page(uint8_t page, uint8_t quant_columm ,uint8_t tipo_exibir);

//6502 CPU registers
extern uint16_t pc;
extern uint8_t sp, a, x, y, cpustatus;


//helper variables
extern uint16_t ea, reladdr, value, result;
extern uint8_t opcode, useaccum;

void display(){

    printf("\n+================================+\n");
    printf("|   PC: %d    OPC: %s", pc,get_comand(RAM[pc]));
    printf("\n+--------------------------------+\n");
    printf("|   A: %d  X: %d  Y: %d", a, x, y);
    printf("\n+--------------------------------+\n|");
    char * str_flags =  get_flags();
    for (uint8_t i = 0; i < 8; i++){
        printf(" %c ", str_flags[i]);
    }
    printf("\n+================================+\n");

    printf("\n+--------------------------------------------------------+\n|  prg code: %d %d %d < %d > %d %d %d  dec", RAM[pc-3], RAM[pc-2], RAM[pc-1], RAM[pc] , RAM[pc+1],RAM[pc+2], RAM [pc+3]);
    printf("\n|  prg code: %x %x %x < %x > %x %x %x  hex", RAM[pc-3], RAM[pc-2], RAM[pc-1], RAM[pc] , RAM[pc+1],RAM[pc+2], RAM [pc+3]);
    printf("\n+--------------------------------------------------------+\n");

}



void display_ram_for_page(uint8_t page, uint8_t quant_columm ,uint8_t tipo_exibir){
    uint16_t adress_start = page * 256;
    uint16_t adress_end = adress_start + 255;
    printf("\n Page: %d", page);
    display_ram_continuo(adress_start,adress_end,quant_columm,tipo_exibir);
}
//mostra um trechoda memoria ram o parametro tipo_exibição e para mostrar os resgistros da ram em hexadecimal = 1, 2 = charcter,,0 = decimal
void display_ram_continuo(uint16_t adress_start, uint16_t adress_end, uint8_t quant_columm,uint8_t tipo_exibir){
    int quebra = 0;
    printf("\n [ RAM memory: %d - %d ]\n",adress_start, adress_end);
    printf("+");
    for(int i = 0; i < quant_columm;i++)
        printf("=====");
    printf("+\n ");
    for(int i = adress_start; i <= adress_end;i++){
        if (tipo_exibir == INTEGER){
            if(RAM[i] < 10)
                printf(" 00%d ",RAM[i]);
            else
                if(RAM[i] < 100)
                    printf(" 0%d ",RAM[i]);
                else printf(" %d ",RAM[i]);

        } else {
            if(tipo_exibir == HEXADECIMAL)printf(" %h",RAM[i]);
            else printf(" %c",RAM[i]);
        }
        if (quebra  == quant_columm -1){
            if ( i != adress_end) printf("\n ");
            quebra = -1;
        }
    quebra++;
    }
    printf("\n+");
    for(int i = 0; i < quant_columm;i++)
        printf("=====");
    printf("+\n ");
}

// função devolve uma string de caracter com as flags da cpu
static char * get_flags(){

    uint8_t tmp_status = cpustatus;
    static char tmp_flag[8] = {' ',' ',' ',' ',' ',' ',' ',' '};

    if((tmp_status % 2))tmp_flag[7] = 'c';  //carry
    if((tmp_status >> 1) % 2)tmp_flag[6] = 'z';// 1=result is 0
    if((tmp_status >> 2) % 2)tmp_flag[5] = 'i'; // 1 = irq disable
    if((tmp_status >> 3) % 2)tmp_flag[4] = 'd'; // decimal mode
    if((tmp_status >> 4) % 2)tmp_flag[3] = 'b'; // 1 = brake comand
    if((tmp_status >> 6) % 2)tmp_flag[1] = 'o'; // overflow 1 = true
    if((tmp_status >> 7) % 2)tmp_flag[0] = 'n'; //negative 1 = neg
    return tmp_flag;
}

// A função recebe o opcode e retorna uma strin informando qual e o comando do opcode;

const char * get_comand( uint8_t opc){
    const char * tmp_str;
    tmp_str = "???";
    //ADC COM CARRY
    tmp_str =  (opc == 0X69)?"ADC #$44":tmp_str;
    tmp_str =  (opc == 0X65)?"ADC $44":tmp_str;
    tmp_str =  (opc == 0X75)?"ADC $44,X":tmp_str;
    tmp_str =  (opc == 0X6D)?"ADC $4400":tmp_str;
    tmp_str =  (opc == 0X7D)?"ADC $4400,X":tmp_str;
    tmp_str =  (opc == 0X79)?"ADC $4400,Y":tmp_str;
    tmp_str =  (opc == 0X61)?"ADC ($44,X)":tmp_str;
    tmp_str =  (opc == 0X71)?"ADC ($44),Y":tmp_str;

    //AND (bitwise AND with accumulator)

    tmp_str =  (opc == 0X29)?"AND #$44":tmp_str;
    tmp_str =  (opc == 0X25)?"AND $44 ":tmp_str;
    tmp_str =  (opc == 0X35)?"AND $44,X":tmp_str;
    tmp_str =  (opc == 0X2D)?"AND $4400":tmp_str;
    tmp_str =  (opc == 0X3D)?"AND $4400,X":tmp_str;
    tmp_str =  (opc == 0X39)?"AND $4400,Y":tmp_str;
    tmp_str =  (opc == 0X21)?"AND ($44,X)":tmp_str;
    tmp_str =  (opc == 0X31)?"AND ($44),Y":tmp_str;

    //ASL (Arithmetic Shift Left)
    tmp_str =  (opc == 0X0A)?"ASL A":tmp_str;
    tmp_str =  (opc == 0X06)?"ASL $44":tmp_str;
    tmp_str =  (opc == 0X16)?"ASL $44,X":tmp_str;
    tmp_str =  (opc == 0X0E)?"ASL $4400":tmp_str;
    tmp_str =  (opc == 0X1E)?"ASL $4400,X":tmp_str;

//BIT (test BITs)
    tmp_str =  (opc == 0X24)?"BIT $44":tmp_str;
    tmp_str =  (opc == 0X2C)?"BIT $4400":tmp_str;

//Branch Instructions

    tmp_str =  (opc == 0X10)?"BPL":tmp_str;
    tmp_str =  (opc == 0X30)?"BMI":tmp_str;
    tmp_str =  (opc == 0X50)?"BVC":tmp_str;
    tmp_str =  (opc == 0X70)?"BVS":tmp_str;
    tmp_str =  (opc == 0X90)?"BCC":tmp_str;
    tmp_str =  (opc == 0XB0)?"BCS":tmp_str;
    tmp_str =  (opc == 0XD0)?"BNE":tmp_str;
    tmp_str =  (opc == 0XF0)?"BEQ":tmp_str;

//BRK (BReaK)
    tmp_str =  (opc == 0X00)?"BRK":tmp_str;


//CMP (CoMPare accumulator)
    tmp_str =  (opc == 0XC9)?"CMP #$44":tmp_str;
    tmp_str =  (opc == 0XC5)?"CMP $44":tmp_str;
    tmp_str =  (opc == 0XD5)?"CMP $44,X":tmp_str;
    tmp_str =  (opc == 0XCD)?"CMP $4400":tmp_str;
    tmp_str =  (opc == 0XDD)?"CMP $4400,X":tmp_str;
    tmp_str =  (opc == 0XD9)?"CMP $4400,Y":tmp_str;
    tmp_str =  (opc == 0XC1)?"CMP ($44,X)":tmp_str;
    tmp_str =  (opc == 0XD1)?"CMP ($44),Y":tmp_str;

//CPX (ComPare X register)
    tmp_str =  (opc == 0XE0)?"CPX #$44":tmp_str;
    tmp_str =  (opc == 0XE4)?"CPX $44":tmp_str;
    tmp_str =  (opc == 0XEC)?"CPX $4400":tmp_str;


//CPY (ComPare Y register)
    tmp_str =  (opc == 0XC0)?"CPY #$44":tmp_str;
    tmp_str =  (opc == 0XC4)?"CPY $44":tmp_str;
    tmp_str =  (opc == 0XCC)?"CPY $4400":tmp_str;

//DEC (DECrement memory)
    tmp_str =  (opc == 0XC6)?"DEC $44":tmp_str;
    tmp_str =  (opc == 0XD6)?"DEC $44,X":tmp_str;
    tmp_str =  (opc == 0XCE)?"DEC $4400":tmp_str;
    tmp_str =  (opc == 0XDE)?"DEC $4400,X":tmp_str;


//EOR (bitwise Exclusive OR)
    tmp_str =  (opc == 0X49)?"EOR #$44":tmp_str;
    tmp_str =  (opc == 0X45)?"EOR $44":tmp_str;
    tmp_str =  (opc == 0X55)?"EOR $44,X":tmp_str;
    tmp_str =  (opc == 0X4D)?"EOR $4400":tmp_str;
    tmp_str =  (opc == 0X5D)?"EOR $4400,X":tmp_str;
    tmp_str =  (opc == 0X59)?"EOR $4400,Y":tmp_str;
    tmp_str =  (opc == 0X41)?"EOR ($44,X)":tmp_str;
    tmp_str =  (opc == 0X51)?"EOR ($44),Y":tmp_str;


//Flag (Processor Status) Instructions
    tmp_str =  (opc == 0X18)?"CLC":tmp_str;
    tmp_str =  (opc == 0X38)?"SEC":tmp_str;
    tmp_str =  (opc == 0X58)?"CLI":tmp_str;
    tmp_str =  (opc == 0X78)?"SEI":tmp_str;
    tmp_str =  (opc == 0XB8)?"CLV":tmp_str;
    tmp_str =  (opc == 0XD8)?"CLD":tmp_str;
    tmp_str =  (opc == 0XF8)?"SED":tmp_str;

//INC (INCrement memory)
    tmp_str =  (opc == 0XE6)?"INC $44":tmp_str;
    tmp_str =  (opc == 0XF6)?"INC $44,X":tmp_str;
    tmp_str =  (opc == 0XEE)?"INC $4400":tmp_str;
    tmp_str =  (opc == 0XFE)?"INC $4400,X":tmp_str;


//JMP (JuMP)
    tmp_str =  (opc == 0X4C)?"JMP $5597":tmp_str;
    tmp_str =  (opc == 0X6C)?"JMP ($5597)":tmp_str;

//JSR (Jump to SubRoutine)

    tmp_str =  (opc == 0X20)?"JSR $5597":tmp_str;


//LDA (LoaD Accumulator)
    tmp_str =  (opc == 0XA9)?"LDA #$44":tmp_str;
    tmp_str =  (opc == 0XA5)?"LDA $44":tmp_str;
    tmp_str =  (opc == 0XB5)?"LDA $44,X":tmp_str;
    tmp_str =  (opc == 0XAD)?"LDA $4400":tmp_str;
    tmp_str =  (opc == 0XBD)?"LDA $4400,X":tmp_str;
    tmp_str =  (opc == 0XB9)?"LDA $4400,Y":tmp_str;
    tmp_str =  (opc == 0XA1)?"LDA ($44,X)":tmp_str;
    tmp_str =  (opc == 0XB1)?"LDA ($44),Y":tmp_str;


//LDX (LoaD X register)
    tmp_str =  (opc == 0XA2)?"LDX #$44":tmp_str;
    tmp_str =  (opc == 0XA6)?"LDX $44":tmp_str;
    tmp_str =  (opc == 0XB6)?"LDX $44,Y":tmp_str;
    tmp_str =  (opc == 0XAE)?"LDX $4400":tmp_str;
    tmp_str =  (opc == 0XBE)?"LDX $4400,Y":tmp_str;


//LDY (LoaD Y register)
    tmp_str =  (opc == 0XA0)?"LDY #$44":tmp_str;
    tmp_str =  (opc == 0XA4)?"LDY $44":tmp_str;
    tmp_str =  (opc == 0XB4)?"LDY $44,X":tmp_str;
    tmp_str =  (opc == 0XAC)?"LDY $4400":tmp_str;
    tmp_str =  (opc == 0XBC)?"LDY $4400,X":tmp_str;


//LSR (Logical Shift Right)
    tmp_str =  (opc == 0X4A)?"LSR A":tmp_str;
    tmp_str =  (opc == 0X46)?"LSR $44":tmp_str;
    tmp_str =  (opc == 0X56)?"LSR $44,X":tmp_str;
    tmp_str =  (opc == 0X4E)?"LSR $4400":tmp_str;
    tmp_str =  (opc == 0X5E)?"LSR $4400,X":tmp_str;


//NOP (No OPeration)
    tmp_str =  (opc == 0XEA)?"NOP":tmp_str;


//ORA (bitwise OR with Accumulator)
    tmp_str =  (opc == 0X09)?"ORA #$44":tmp_str;
    tmp_str =  (opc == 0X05)?"ORA $44":tmp_str;
    tmp_str =  (opc == 0X15)?"ORA $44,X":tmp_str;
    tmp_str =  (opc == 0X0D)?"ORA $4400":tmp_str;
    tmp_str =  (opc == 0X1D)?"ORA $4400,X":tmp_str;
    tmp_str =  (opc == 0X19)?"ORA $4400,Y":tmp_str;
    tmp_str =  (opc == 0X01)?"ORA ($44,X)":tmp_str;
    tmp_str =  (opc == 0X11)?"ORA ($44),Y":tmp_str;


//Register Instructions
    tmp_str =  (opc == 0XAA)?"TAX":tmp_str;
    tmp_str =  (opc == 0X8A)?"TXA":tmp_str;
    tmp_str =  (opc == 0XCA)?"DEX":tmp_str;
    tmp_str =  (opc == 0XE8)?"INX":tmp_str;
    tmp_str =  (opc == 0XA8)?"TAY":tmp_str;
    tmp_str =  (opc == 0X98)?"TYA":tmp_str;
    tmp_str =  (opc == 0X88)?"DEY":tmp_str;
    tmp_str =  (opc == 0XC8)?"INY":tmp_str;



//ROL (ROtate Left)
    tmp_str =  (opc == 0X2A)?"ROL A":tmp_str;
    tmp_str =  (opc == 0X26)?"ROL $44":tmp_str;
    tmp_str =  (opc == 0X36)?"ROL $44,X":tmp_str;
    tmp_str =  (opc == 0X2E)?"ROL $4400":tmp_str;
    tmp_str =  (opc == 0X3E)?"ROL $4400,X":tmp_str;


//ROR (ROtate Right)
    tmp_str =  (opc == 0X6A)?"ROR A":tmp_str;
    tmp_str =  (opc == 0X66)?"ROR $44":tmp_str;
    tmp_str =  (opc == 0X76)?"ROR $44,X":tmp_str;
    tmp_str =  (opc == 0X6E)?"ROR $4400":tmp_str;
    tmp_str =  (opc == 0X7E)?"ROR $4400,X":tmp_str;

//RTI (ReTurn from Interrupt)
    tmp_str =  (opc == 0X40)?"RTI":tmp_str;


//RTS (ReTurn from Subroutine)
    tmp_str =  (opc == 0X60)?"RTS":tmp_str;

//SBC (SuBtract with Carry)
    tmp_str =  (opc == 0XE9)?"SBC #$44":tmp_str;
    tmp_str =  (opc == 0XE5)?"SBC $44":tmp_str;
    tmp_str =  (opc == 0XF5)?"SBC $44,X":tmp_str;
    tmp_str =  (opc == 0XED)?"SBC $4400":tmp_str;
    tmp_str =  (opc == 0XFD)?"SBC $4400,X":tmp_str;
    tmp_str =  (opc == 0XF9)?"SBC $4400,Y":tmp_str;
    tmp_str =  (opc == 0XE1)?"SBC ($44,X)":tmp_str;
    tmp_str =  (opc == 0XF1)?"SBC ($44),Y":tmp_str;



    //STA (STore Accumulator)
    tmp_str =  (opc == 0X85)?"STA $44":tmp_str;
    tmp_str =  (opc == 0X95)?"STA $44,X":tmp_str;
    tmp_str =  (opc == 0X8D)?"STA $4400":tmp_str;
    tmp_str =  (opc == 0X9D)?"STA $4400,X":tmp_str;
    tmp_str =  (opc == 0X99)?"STA $4400,Y":tmp_str;
    tmp_str =  (opc == 0X81)?"STA ($44,X)":tmp_str;
    tmp_str =  (opc == 0X91)?"STA ($44),Y":tmp_str;

    //Stack Instructions

    tmp_str =  (opc == 0X9A)?"TXS":tmp_str;
    tmp_str =  (opc == 0XBA)?"TSX":tmp_str;
    tmp_str =  (opc == 0X48)?"PHA":tmp_str;
    tmp_str =  (opc == 0X68)?"PLA":tmp_str;
    tmp_str =  (opc == 0X08)?"PHP":tmp_str;
    tmp_str =  (opc == 0X28)?"PLP":tmp_str;


    //STX (STore X register)
    tmp_str =  (opc == 0X86)?"STX $44":tmp_str;
    tmp_str =  (opc == 0X96)?"STX $44,Y":tmp_str;
    tmp_str =  (opc == 0X8E)?"STX $4400":tmp_str;


    //STY (STore Y register)
    tmp_str =  (opc == 0X84)?"STY $44":tmp_str;
    tmp_str =  (opc == 0X94)?"STY $44,X":tmp_str;
    tmp_str =  (opc == 0X8C)?"STY $4400":tmp_str;
    return tmp_str;
}
