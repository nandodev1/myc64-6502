FILE * basic_room = fopen("basic_c65.bin","r");
int tamanho = 8 * 1024;
uint8_t * buffer = (uint8_t *) malloc(tamanho * sizeof(uint8_t));
size_t elementos_lidos = fread(buffer, 1, tamanho, basic_room);

const /*PROGMEM*/ unsigned char * basic_c65_prg = buffer;
unsigned int basic_c65_prg_len = 8 * 1024;

