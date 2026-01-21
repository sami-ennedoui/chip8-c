#include <stdint.h>
//the main struct which will be used throught the project
typedef struct {
	uint8_t memory[4096];
	uint8_t v[16];
	uint16_t I;   //index register
	uint16_t pc; // counter
	uint8_t delay_timer;
    uint8_t sound_timer;
    uint16_t stack[16];
	uint8_t sp;       //stack pointer
	uint8_t display[64*32]; //display pixels
}chip8;	
