#include <iostream>
#include "util.h"
#include"mult10_prg.h"
#include "room/ciao.h"
#include<errno.h>
#include <time.h>


using namespace std;


void pause (float delay1) {

   if (delay1<0.001) return; // pode ser ajustado e/ou evita-se valores negativos.

   float inst1=0, inst2=0;

   inst1 = (float)clock()/(float)CLOCKS_PER_SEC;

   while (inst2-inst1<delay1) inst2 = (float)clock()/(float)CLOCKS_PER_SEC;

   return;

}

int main()
{
    cout << "Iniciando debug..." << endl;
    uint16_t quant_inst = 1;//define quantas instruçoes vai ser executada com o comando e (execute);
    reset6502();
    //load_prg(builtinprg_1_prg,0X8000,builtinprg_1_prg_len);
    //load_file_prg("room/kernal.bin",0XE000,0XFFFF-0XE000);
    //load_file_prg("room/Escape_to_Freedom.prg",0X5000,28* 1024);
    uint16_t inicio_rotina = 0XFDA3;
    pc = inicio_rotina;
    uint8_t ram_page_display = 2;
    bool run = false;//revisar
    uint8_t tipo_exibicao = INTEGER;
    for(;;){
        getchar();
        display();
        display_ram_for_page(ram_page_display,16,tipo_exibicao);
        //display_ram_for_page(ram_page_display+1,16,tipo_exibicao);
        char comando;
        cin >> comando ;
        if (comando == 'q')break;//Sai do debugador
        if (comando == 's'){//Configura quntidade e instrucoes à ser executada.
            cin >> quant_inst;
        }
        if (comando == 'n')ram_page_display++;//Avança uma pagina de memoria
        if (comando == 'p')ram_page_display--;//recua uma pagina de memoria.
        if (comando == 'j')ram_page_display+=10;//Pula 10 paginas de memoria apartir da atual
        if (comando == 'v')ram_page_display-=10;//Recua 10 paginas de memoria ram apartir da atual.
        if (comando == 'e')//Executa instrução.
            for(uint16_t i = 0; i < quant_inst;i++)
                exec6502();
        if (comando == 'i'){//Roda o programa
            while(true){
                exec6502();
                display();
                display_ram_for_page(ram_page_display,16,tipo_exibicao);
                pause(0.08);
                static char * flags = get_flags();
                if(flags[5] == 'i') break;
            }
            cpustatus = 0;
        }
        if(comando == 's'){
            cout << "Defina qunt. instrução: ";
            cin >> quant_inst;
            cout << quant_inst;
        }
        if (comando == 'g'){//vai ate a pagina da ram selecionada
            cout << "Digite a page: " ;
            int tmp;
            cin >> tmp;
            ram_page_display = (uint8_t)tmp;
        }
        if (comando == 'r'){//reseta
            reset6502();
            pc = inicio_rotina;
        }
        if(comando == 't'){//seleciona como vai ser exibida a ram
            cout << "Digite o tipo (c,h,d): ";
            cin >> comando;
            switch(comando){

                case 'c':
                    tipo_exibicao = CHARACTER;
                break;

                case 'h':
                    tipo_exibicao = INTEGER;
                break;
                case 'd':
                    tipo_exibicao = HEXADECIMAL;
                break;
                default:
                    cout << "Digite um valor valido";
            }
        }
        if (comando == 'h'){
            printf("\n q: Sai do debugador.");
            printf("\n n: Avança uma pagina de memoria.");
            printf("\n p: Recua uma pagina de memoria.");
            printf("\n j: Pula 10 paginas de memoria apartir da atual.");
            printf("\n v: Recua 10 paginas de memoria ram apartir da atual.");
            printf("\n e: Executa instrução.");
            printf("\n i: Roda programa ate haver uma interrupção.");
            printf("\n s: Configura quntidade e instrucoes à ser executada.");
            printf("\n g: Vai ate a pagina da ram selecionada.");
            printf("\n r: Resetar.");
            printf("\n t: Define tipo de exibição da ram.");
            printf("\n h: Exibe esta mensagem.\n");
        }
        //printf("\nDigite comando( ? Ajuda ): ");
    }
    return 0;
}
