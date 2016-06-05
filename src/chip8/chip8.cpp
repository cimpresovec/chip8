//
// Created by luka on 22.5.2016.
//

#include <fstream>
#include "chip8.h"

Chip8::Chip8()
{
    //Copy font data to RAM
    std::copy(font_data.begin(), font_data.end(), ram.begin());
}

void Chip8::loadROM(const std::string file_name)
{
    //Open file
    std::ifstream file (file_name, std::ios_base::in | std::ios_base::binary);

    if (file.is_open())
    {
        //Read in ram @ 0x200
        unsigned short pos = 0x200;

        //Ready the bytes
        char b;
        while (!file.eof())
        {
            file.get(b);
            ram[pos] = (unsigned char)b;
            pos++;
        }
    }
    file.close();
}

void Chip8::tick()
{
    //Fetch opcode
    unsigned short opcode = ram[PC] << 8 | ram[PC + 1];


    switch (opcode & 0xF000)
    {
        //3 ops
        case 0x0000:
            switch (opcode & 0x00FF)
            {
                //CLS - clear screen
                case 0x00E0:
                    //TODO
                    break;
                //RET - return from subroutine
                case 0x00EE:
                    //TODO
                    break;
                //SYS addr - jump to 0nnn
                default:
                    PC = opcode & 0x0FFF;
                    break;
            }
            break;
        //JP addr
        case 0x1000:
            PC = opcode & 0x0FFF;
            break;
        //CALL addr
        case 0x2000:
            SP++;
            stack[SP] = PC;
            PC = opcode & 0x0FFF;
            break;
        //SE Vx, byte - 3xbb - Skip next instruction if Vx = byte
        case 0x3000:
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            {
                PC += 2;
            }
            PC += 2;
            break;
        //SNE Vx, byte - 4xbb - Not equal skip
        case 0x4000:
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            {
                PC += 2;
            }
            PC += 2;
            break;
        //SE Vx, Vy - 5xy0 - Equal skip
        case 0x5000:
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
            {
                PC += 2;
            }
            PC += 2;
            break;
        //LD Vx, byte - put byte in Vx
        case 0x6000:
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            PC += 2;
            break;
        //ADD Vx, byte - Set Vx = Vx + kk.
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            break;
        default:
            //TODO ERROR
            break;
    }
}
