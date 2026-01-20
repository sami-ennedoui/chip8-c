#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "chip8.h"
#define FONT_START 0x050
#define START_ROM 0x200
 //most programs expect the fonts and roms to not be at the start of the memory so we introduce a slight offset
unsigned char chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F
};
void chip8_init (chip8* c){
    memset(c,0,sizeof(*c));
    c->pc = 0x200;
    for(int i=0;i<80;i++){
        c->memory[i + FONT_START] = chip8_fontset[i];
    }
}
short chip8_load_ROM(chip8 *c, const char *rom_path) {
    FILE *f = fopen(rom_path, "rb");
    if (f == NULL) {
        printf("ERROR: could not open ROM file\n");
        return 0;
    }

    const size_t start = 0x200;
    size_t offset = 0;
    int byte;

    while ((byte = fgetc(f)) != EOF) {
        if (start + offset >= sizeof c->memory) {
            printf("ERROR: ROM too large for memory\n");
            fclose(f);
            return 0;
        }
        c->memory[start + offset] = (uint8_t)byte;
        offset++;
    }

    fclose(f);
    return 1;
}
int main(void) {
    chip8 chip;
    chip8_init(&chip);
    chip8_load_ROM(&chip,IBM_Logo.ch8);
    return 0;
}