//
// Created by luka on 22.5.2016.
//

#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include <array>
#include <string>
#include <random>

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
    unsigned char DT = 0;
    unsigned char ST = 0;

    //Stack
    unsigned char SP = 0;
    std::array<unsigned short, 16> stack {};

    //Display - accessed with [y][x]
    std::array< std::array<unsigned char, 64>, 32> display {};

    //Input
    std::array<unsigned char, 16> input {};
    unsigned char last_pressed = 0;
    bool new_press = false;
    bool need_new = false;

    //Methods
    Chip8();
    void tick();
    void tickTimers();
    void loadROM(const std::string file_name);

    //Random engine
    std::random_device random_engine {};
};

#endif //CHIP8_CHIP8_H
