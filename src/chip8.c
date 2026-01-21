#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "chip8.h"
#define FONT_START 0x050
#define START_ROM 0x200
#define MAX_SIZE 0xFFF-START_ROM
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
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //Egh
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
void chip8_draw_text(const chip8 *c) {
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            int index = y * 64 + x;
            putchar(c->display[index] ? 'c' : ' ');// prints # if the display pixel is set to 1, places a space if not
        }
        putchar('\n');
    }
}
void chip8_step(chip8 *c){
    //fetching the instructions from memory
    uint16_t opcode = (c->memory[c->pc] << 8 | c->memory[c->pc+1]);
    c->pc += 2;
    //decoding the instructions
    uint16_t nnn = opcode & 0x0FFF; // takes the first 12 bits (is 12 bytes in size the rest is only 4 or 8 bytes long) 
    uint8_t n = opcode & 0x000F; // takes the first 4 bits
    uint8_t kk = opcode & 0x00FF; //takes the first 8 bits
    uint8_t x = (opcode & 0x0F00) >> 8;// takes the first 4 bits of the high byte
    uint8_t y = (opcode & 0x00F0) >> 4; //takes the last 4 bits of the low byte
    //executing the instructions
    switch (opcode & 0xF000){
        case 0x0000 : 
            switch (opcode) {
                case 0x00E0: // CLS 
                    memset(c->display,0,sizeof(c->display));
                    break;
                case 0x00EE: // RET
                    c->sp--;
                    c->pc = c->stack[c->sp];
                    break;
                default: break;
        }
        break;
        case 0x1000 : // JP addr 
            c->pc = nnn;
            break;
        case 0x2000 : //CALL addr 
            c->stack[c->sp] = c->pc;
            c->sp ++;
            c->pc = nnn;
            break;
        case 0x3000:    // sSE Vx, byte
            if (c->v[x]== kk){
                c->pc += 2;
            }
            break;
        case 0x4000 :
            if (c->v[x] != kk){
                c->pc += 2;
            }
            break;
        case 0x5000 :
            if (c->v[x] == c->v[y]){
                c->pc += 2;
            }
            break;
        case 0x6000 :
            c->v[x] = kk;
            break;
        case 0x7000 :
            c->v[x] = c->v[x] + kk;
            break;
        case 0x8000 :
            switch(opcode & 0x000F){
                case 0x1 : 
                    c->v[x] = c->v[x] | c->v[y];
                    break;
                case 0x2 : 
                    c->v[x] = c->v[x] & c->v[y];
                    break;
                case 0x3 :
                    c->v[x] = c->v[x] ^ c->v[y];
                    break;
                case 0x4 :
                    uint16_t sum = c->v[x] = c->v[y] + c->v[x];
                    c->v[0xF] = (sum > 0xFF);
                    c->v[x] = (uint8_t)sum;
                    break;
                case 0x5 :
                    if (c->v[y] < c->v[x]){
                        c->v[0xF] = 1;
                    }
                    else {
                        c->v[0xF] = 0;
                    }
                    c->v[x] = c->v[x]  -c->v[y];
                    break;
                case 0x6 : 
                    c->v[0xF] = c->v[x] & 0x1;
                    c->v[x] >>= 1;
                    break;
                case 0x7 :
                     if (c->v[y] > c->v[x]){
                        c->v[0xF] = 1;
                    }
                    else {
                        c->v[0xF] = 0;
                    }
                    c->v[x] = c->v[y] - c->v[x];
                    break;
                case 0xE :
                    c->v[0xF] = (c->v[x] & 0x80) >> 7;
                    c->v[x] <<= 1;
                    break;
                case 0x0:
                    c->v[x] = c->v[y];
                    break;
            }
            break;
        case 0x9000 : 
            if (c->v[x] != c->v[y]){
                c->pc += 2;
            }
            break;
        case 0xA000 : 
            c->I = nnn;
            break;
        case 0xB000 :
            c->pc = c->v[0] + nnn;
            break;
        case 0xC000 :
            c->v[x] = (rand()%256) & kk;
            break;
        case 0xD000 : 
            for (int h = 0 ; h<n;h++){
                uint8_t sprite_byte = c->memory[c->I + h];
                for (int u = 0; u<8 ; u++){
                    uint8_t sprite_pixel = (sprite_byte >> (7 - u)) & 1;//extracting the 7-u th bit to get a single bit (1 or 0)
                    if (sprite_pixel) {
                        int y = (c->v[y] + h) % 32; // % handles the wrapping of the pixels in case it excceeds the limits of the screen
                        int x = (c->v[x] + u) % 64;
                        int index = y * 64 + x;
                        uint8_t old_pixel = c->display[index];
                         c->display[index] ^= 1;
                        if (old_pixel == 1 && c->display[index] == 0){
                             c->v[0xF] = 1;//checks for collisions
                        }
                    }
                }                
            }
        break;
        case 0xE000:
            if ((opcode & 0x00FF) == 0x009E){//SKP
                if(c->keys[c->v[x]] == 1){
                    c->pc += 2;
                }
            } 
            else if((opcode & 0x00FF) == 0x00A1){//SKNP
                if(c->keys[c->v[x]] == 0){
                    c->pc += 2;
                }
            }
            break;
        case 0xF000: 
            switch(opcode & 0x00FF){
                case 0x07: 
                    c->v[x] = c->delay_timer;
                    break;
                case 0x0A:{
                    int pressed = -1 ;//maintains the while loop while a key is not yet pressed
                    for(int p = 0 ; p<16 ; p++){
                        if(c->keys[p]){pressed = p; break;}
                    }
                    if(pressed == -1){
                        c->pc -= 2;
                    }
                    else {
                        c->v[x] = (uint8_t)pressed;
                    }
                    break;
            }
                case 0x15: 
                    c->delay_timer = c->v[x];
                    break;
                case 0x18:
                    c->sound_timer = c->v[x];
                    break;
                case 0x1E:
                    c->I += c->v[x];
                    break;
                case 0x29:
                    c->I = FONT_START + (c->v[x]*5); //LD F,Vx
                    break;
                case 0x33:
                    uint8_t value = c->v[x];
                    c->memory[c->I + 2] = value % 10; // ones
                    c->memory[c->I + 1] = (value / 10) % 10;  // tens
                    c->memory[c->I] = (value / 100) % 10; // hundreds
                    break;
                case 0x55:
                    for(int u = 0; u<= x;u++){
                        c->memory[c->I+u] = c->v[u];
                    }
                    break;
                case 0x65:
                    for(int u = 0; u<= x ;u++){
                        c->v[u] = c->memory[c->I+u];
                    }
                    break;
                default: break;
            }
            break;
        default:    break;
    }
    }