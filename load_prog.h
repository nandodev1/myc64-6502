
#ifdef
#include "builtinprg_1.h"
#endif

int load_file_prg(const char * file_prog, uint16_t adress, unsigned int len){
    FILE * room = fopen(file_prog,"r");
    uint8_t * buffer = (uint8_t *) malloc(len * sizeof(uint8_t));
    size_t elementos_lidos = fread(buffer, 1, len, room);
    fclose(room);
    load_prg(room, adress,len);
    return 0;
}
