//
// Created by luka on 22.5.2016.
//

#include <fstream>
#include <iostream>
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

void Chip8::tickTimers()
{
    if (DT > 0) DT--;
    if (ST > 0) ST--;
}

void Chip8::tick()
{
    //Fetch opcode
    unsigned short opcode = ram[PC] << 8 | ram[PC + 1];

    //std::cout << std::hex << opcode << '\n';

    switch (opcode & 0xF000)
    {
        //3 ops
        case 0x0000:
            switch (opcode & 0x00FF)
            {
                //CLS - clear screen
                case 0x00E0:
                    for (unsigned char y = 0; y < 32; ++y)
                    {
                        for (unsigned char x = 0; x < 64; ++x)
                        {
                            display[y][x] = 0;
                        }
                    }
                    PC += 2;
                    break;
                //RET - return from subroutine
                case 0x00EE:
                    SP--;
                    PC = stack[SP];
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
            stack[SP] = PC;
            SP++;
            PC = opcode & 0x0FFF;
            break;
        //SE Vx, byte - 3xbb - Skip next instruction if Vx = byte
        case 0x3000:
            if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
            {
                PC += 4;
            }
            else
            {
                PC += 2;
            }
            break;
        //SNE Vx, byte - 4xbb - Not equal skip
        case 0x4000:
            if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
            {
                PC += 4;
            }
            else
            {
                PC += 2;
            }
            break;
        //SE Vx, Vy - 5xy0 - Equal skip
        case 0x5000:
            if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
            {
                PC += 4;
            }
            else
            {
                PC += 2;
            }
            break;
        //LD Vx, byte - put byte in Vx
        case 0x6000:
            V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
            PC += 2;
            break;
        //ADD Vx, byte - Set Vx = Vx + kk.
        case 0x7000:
            V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
            PC += 2;
            break;
        //Multiple op codes
        case 0x8000:
            switch (opcode & 0x000F)
            {
                //LD Vx Vy, store Vy in Vx
                case 0x0:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;
                //OR Vx Vy, store in Vx
                case 0x1:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;
                //AND Vx, Vy, store in Vx
                case 0x2:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;
                //XOR Vx, Vy, store in Vx
                case 0x3:
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;
                //ADD Vx, Vy, store in Vx, set carry in VF
                case 0x4:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] > 0xFF - V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;
                //SUB Vx, Vy, store in Vx, set VF to NOT burrow
                case 0x5:
                    V[0xF] = V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4];
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
                    PC += 2;
                    break;
                //SHR Vx, VF = Vy & 0x80, Vx = Vy >> 1
                case 0x6:
                    V[0xF] = V[(opcode & 0x00F0) >> 4] & 0x1;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] >> 1;
                    PC += 2;
                    break;
                //SUBN Vx, Vy, set VF to NOT burrow
                case 0x7:
                    V[0xF] = V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8];
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;
                //SHL Vx, VF = Vy & 0x1, Vx = Vy << 1
                case 0xE:
                    V[0xF] = (V[(opcode & 0x00F0) >> 4] & 0x80) >> 7;
                    V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] << 1;
                    PC += 2;
                    break;
                default:
                    break;
            }
            break;
        //SNE Vx, Vy - skip if Vx != Vy
        case 0x9000:
            if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
            {
                PC += 4;
            }
            else
            {
                PC += 2;
            }
            break;
        //LD I, addr - I = nnn
        case 0xA000:
            I = opcode & 0x0FFF;
            PC += 2;
            break;
        //JP V0, addr - jump to loc V0 + nnn
        case 0xB000:
            PC = (opcode & 0x0FFF) + V[0];
            break;
        //RND Vx, byte - Vx = RANDOM & kk
        case 0xC000:
            V[(opcode & 0x0F00) >> 8] = random_engine() & (opcode & 0x00FF);
            break;
        //DRW Vx, Vy, nibble - draw n-byte sprite at memory location I on position (Vx, Vy), set VF = collision
        case 0xD000:
            //Get parameters
            x_pos = (opcode & 0x0F00) >> 8;
            y_pos = (opcode & 0x00F0) >> 4;
            sprite_size = opcode & 0x000F;

            //Reset VF
            V[0xF] = 0;

            //Wrap position if required
            x_pos = x_pos % 64;
            y_pos = y_pos % 32;

            //Go through sprite rows
            for (unsigned char y = 0; y < sprite_size; ++y)
            {
                //Fetch sprite data
                sprite_byte = ram[I + y];

                for (unsigned char x = 0; x < 8; ++x)
                {
                    //If current sprite bit is set
                    if ((sprite_byte & (0x80 >> x)) == (0x80 >> x))
                    {
                        //If we are going to erase pixel, set VF
                        if (display[y_pos][x_pos] == 1)
                        {
                            V[0xF] = 1;
                        }
                        display[y_pos][x_pos] ^= 1;

                    }
                    //Move position and wrap
                    x_pos = (x_pos + 1) % 64;
                }
                //Move and wrap
                if (x_pos - 8 < 0)
                {
                    x_pos = x_pos - 10 + 64;
                }
                else
                {
                    x_pos = (x_pos - 8) % 64;
                }
                y_pos = (y_pos + 1) % 32;
            }
            PC += 2;
            break;
        case 0xE000:
            switch (opcode & 0x00FF)
            {
                //Skip next if key with hex VX is pressed
                case 0x009E:
                    if (input[V[(opcode & 0x0F00) >> 8]] == 0x1)
                    {
                        PC += 4;
                    }
                    else
                    {
                        PC += 2;
                    }
                    break;
                //Skip next if key with hex Vx is not pressed
                case 0x00A1:
                    if (input[V[(opcode & 0x0F00) >> 8]] == 0x0)
                    {
                        PC += 4;
                    }
                    else
                    {
                        PC += 2;
                    }
                    break;
                default:
                    //TODO ERROR
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF)
            {
                //Store value of delay timer in VX
                case 0x0007:
                    V[(opcode & 0x0F00) >> 8] = DT;
                    PC += 2;
                    break;
                //Wait for keypress and store result in VX
                case 0x000A:
                    //TODO IMPLEMENT, TEMP
                    V[(opcode & 0x0F00) >> 8] = 0x0;
                    PC += 2;
                    break;
                //Set delay timer to VX
                case 0x0015:
                    DT = V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;
                //Set sound timer to VX
                case 0x0018:
                    ST = V[(opcode & 0x0F00) >> 8];
                    PC =+ 2;
                    break;
                //Set I = I + VX
                case 0x001E:
                    I += V[(opcode & 0x0F00) >> 8];
                    PC += 2;
                    break;
                //Set I to mem address of sprite of char stored in VX
                case 0x0029:
                    //Each char is 5 bytes of data, starting at 0x00 in RAM
                    I = V[(opcode & 0x0F00) >> 8] * 5;
                    PC += 2;
                    break;
                //Store binary coded equivalent of value in VX to I, I + 1, I + 2
                case 0x0033:
                    //TODO IMPLEMENT
                    PC += 2;
                    break;
                //Store values of registers V0 - VX inclusive in memory starting at I, move I to the ned
                case 0x0055:
                {

                    unsigned char count = (opcode & 0x0F00) >> 8;
                    for (unsigned char x = 0; x < count; ++x)
                    {
                        ram[I] = V[x];
                        I++;
                    }
                    PC += 2;
                    break;
                }
                //Fill values of registers V0 - Vx inclusive with memory starting at I, move I to the end
                case 0x0065:
                {
                    unsigned char cunt = (opcode & 0x0F00) >> 8;
                    for (unsigned char x = 0; x < cunt; ++x)
                    {
                        V[x] = ram[I];
                        I++;
                    }
                    PC += 2;
                    break;

                }
                default:
                    //TODO ERROR
                    break;
            }
            break;
        default:
            //TODO ERROR
            break;
    }
}
