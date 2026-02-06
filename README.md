# chip8-c
### What is chip8?
Chip8 is an interpreted programming language, made in the 1970s with the goal of being less memory intensive than other languages that were used at the time like BASIC. ( Chip8 needed only 4 kilobytes of memory to run programs the same as the APOLLO guidance computer!).
The language has been used in many applications like video game consoles, home computers, calculators and so on.
### Project overview
This project was created with the goal of making a spec-compliant chip8 emulator/virtual machine with support for its modern opcodes, monochrome graphics and keyboard inputs. The project aims to test and increase my understanding of the C programming language and my knowledge of computer architecture. This project has been successful so far in providing the MVP I intended but improvements are still to be done in the future ( for instance audio support) 
## Feature set
- full instruction set support to the chip8 standart
- 4kB of memory, 16 registers named V0 through VF, a 16 bit index, stack support for calls and returns as well as a program counter.
- keyboard support tested using the keypad_test.ch8 ROM.
- Full support for .ch8 ROMs.
## Tools used
- programming language used: C.
- unit tests using Ctest.
- Graphics using the SDL3 libary.
clone the repo : 
## Changing the ROM file
For now, to change the rom file you need to change it from the main.c file located in the test directory, specifically in **line 68** to do that
First clone the repo:
`git clone https://github.com/sami-ennedoui/chip8.git` </br>
Change into the chip8 directory: </br>
`cd chip8`</br>
Navigate to the **test** directory:</br>
`cd test`</br>
Using your text editor of choice you can change the line 68 to the rom of you want to load.</br>
Now use the Mafile to compile to code</br>
`make ..`</br>
you may then run the file named `chip8_sdl.exe` using `./chip8_sdl.exe`</br>
To update the .exe in the game directory use:</br>
`unzip ../game -d ../. && mv chip8_sdl.exe ../game`</br>
You can now run the game using:</br>
`cd ../game && ./chip8_sdl.exe`</br>
