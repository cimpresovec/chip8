//
// Created by luka on 22.5.2016.
//

#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include <array>
#include <string>

//chip8 box
struct Chip8
{
    //Memory
    std::array<unsigned char, 4096> ram {};

    //Font data
    std::array<unsigned char, 80> font_data
            {{
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
             }};

    //Registers
    std::array<unsigned char, 16> V {};
    unsigned short I = 0;
    unsigned short PC = 0x200;

    //Timer registers
    unsigned short DT = 0;
    unsigned short ST = 0;

    //Stack
    unsigned char SP = 0;
    std::array<unsigned short, 16> stack {};

    //Display
    std::array<unsigned short, 64*32> display {};

    //Methods
    Chip8();
    void tick();
    void loadROM(const std::string file_name);
};

#endif //CHIP8_CHIP8_H
