//
// Created by luka on 5.6.2016.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "../chip8/chip8.h"

TEST_CASE( "chip8 interpreter tests", "[chip8]")
{
    Chip8 chip8 {};

    SECTION("JP")
    {
        chip8.ram[0x200] = 0x1B;
        chip8.ram[0x201] = 0xEC;

        chip8.tick();

        REQUIRE(chip8.PC == 0xBEC);
    }
    SECTION("CALL")
    {
        chip8.ram[0x200] = 0x2A;
        chip8.ram[0x201] = 0xAA;

        chip8.tick();

        REQUIRE(chip8.PC == 0xAAA);
        REQUIRE(chip8.SP == 1);
        REQUIRE(chip8.stack[0] == 0x200);
    }
    SECTION("SE")
    {
        chip8.ram[0x200] = 0x3A;
        chip8.ram[0x201] = 0xBB;

        SECTION("equal")
        {
            chip8.V[0xA] = 0xBB;

            chip8.tick();

            REQUIRE(chip8.PC == 0x200 + 4);
        }
        SECTION("not equal")
        {
            chip8.tick();

            REQUIRE(chip8.PC == 0x200 + 2);
        }
    }
    SECTION("SNE")
    {
        chip8.ram[0x200] = 0x4B;
        chip8.ram[0x201] = 0xCC;

        SECTION("equal")
        {
            chip8.V[0xB] = 0xCC;

            chip8.tick();

            REQUIRE(chip8.PC == 0x200 + 2);
        }
        SECTION("not equal")
        {
            chip8.tick();

            REQUIRE(chip8.PC == 0x200 + 4);
        }
    }
    SECTION("SE Vx, Vy")
    {
        chip8.ram[0x200] = 0x56;
        chip8.ram[0x201] = 0x70;

        SECTION("equal")
        {
            chip8.V[0x6] = 0xCC;
            chip8.V[0x7] = 0xCC;

            chip8.tick();

            REQUIRE(chip8.PC == 0x200 + 4);
        }
        SECTION("not equal")
        {
            chip8.V[0x6] = 0xCC;
            chip8.V[0x7] = 0xCB;

            chip8.tick();

            REQUIRE(chip8.PC == 0x200 + 2);
        }
    }
    SECTION("LD Vx, byte")
    {
        chip8.ram[0x200] = 0x60;
        chip8.ram[0x201] = 0x59;

        chip8.tick();

        REQUIRE(chip8.V[0x0] == 0x59);
        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("ADD Vx, byte")
    {
        chip8.ram[0x200] = 0x72;
        chip8.ram[0x201] = 0x01;

        chip8.V[0x2] = 0x05;

        chip8.tick();

        REQUIRE(chip8.V[0x2] == 0x05 + 1);
        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("LD Vx, Vy")
    {
        chip8.ram[0x200] = 0x88;
        chip8.ram[0x201] = 0xC0;

        chip8.V[0xC] = 0xAB;

        chip8.tick();

        REQUIRE(chip8.V[0x8] == 0xAB);
        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("OR Vx, Vy")
    {
        chip8.ram[0x200] = 0x89;
        chip8.ram[0x201] = 0xA1;

        chip8.V[0x9] = 0xAB;
        chip8.V[0xA] = 0x57;

        chip8.tick();

        REQUIRE(chip8.V[0x9] == (0xAB | 0x57));
        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("AND Vx, Vy")
    {
        chip8.ram[0x200] = 0x89;
        chip8.ram[0x201] = 0xA2;

        chip8.V[0x9] = 0xAB;
        chip8.V[0xA] = 0x57;

        chip8.tick();

        REQUIRE(chip8.V[0x9] == (0xAB & 0x57));
        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("XOR Vx, Vy")
    {
        chip8.ram[0x200] = 0x89;
        chip8.ram[0x201] = 0xA3;

        chip8.V[0x9] = 0xAB;
        chip8.V[0xA] = 0x57;

        chip8.tick();

        REQUIRE(chip8.V[0x9] == (0xAB ^ 0x57));
        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("ADD Vx, Vy")
    {
        chip8.ram[0x200] = 0x8A;
        chip8.ram[0x201] = 0xB4;

        chip8.V[0xA] = 0xAA;

        SECTION("overflow")
        {
            chip8.V[0xB] = 0x22;

            chip8.tick();

            REQUIRE(chip8.V[0xA] == 0xAA + 0x22);
            REQUIRE(chip8.V[0xF] == 0);
        }
        SECTION("no overflow")
        {
            chip8.V[0xB] = 0xDD;

            chip8.tick();

            REQUIRE(chip8.V[0xA] == ((0xAA + 0xDD) & 0xFF));
            REQUIRE(chip8.V[0xF] == 1);
        }

        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("SUB Vx, Vy")
    {
        chip8.ram[0x200] = 0x8A;
        chip8.ram[0x201] = 0xB5;

        chip8.V[0xA] = 0xAA;

        SECTION("underflow")
        {
            chip8.V[0xB] = 0xAB;

            chip8.tick();

            REQUIRE(chip8.V[0xA] == 0xFF);
            REQUIRE(chip8.V[0xF] == 0);
        }
        SECTION("no underflow")
        {
            chip8.V[0xB] = 0x11;

            chip8.tick();

            REQUIRE(chip8.V[0xA] == (0xAA - 0x11));
            REQUIRE(chip8.V[0xF] == 1);
        }

        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("SHR Vx")
    {
        chip8.ram[0x200] = 0x8C;
        chip8.ram[0x201] = 0xB6;

        SECTION("bit set")
        {
            chip8.V[0xC] = 0x53;

            chip8.tick();

            REQUIRE(chip8.V[0xF] == 1);
            REQUIRE(chip8.V[0xC] == 0x53 / 2);
        }
        SECTION("bit unset")
        {
            chip8.V[0xC] = 0x52;

            chip8.tick();

            REQUIRE(chip8.V[0xF] == 0);
            REQUIRE(chip8.V[0xC] == 0x52 / 2);
        }

        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("SUBN Vx, Vy")
    {
        chip8.ram[0x200] = 0x8A;
        chip8.ram[0x201] = 0xB7;

        chip8.V[0xB] = 0xAA;

        SECTION("underflow")
        {
            chip8.V[0xA] = 0xAB;

            chip8.tick();

            REQUIRE(chip8.V[0xA] == 0xFF);
            REQUIRE(chip8.V[0xF] == 0);
        }
        SECTION("no underflow")
        {
            chip8.V[0xA] = 0x11;

            chip8.tick();

            REQUIRE(chip8.V[0xA] == (0xAA - 0x11));
            REQUIRE(chip8.V[0xF] == 1);
        }

        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("SHL Vx")
    {
        chip8.ram[0x200] = 0x8C;
        chip8.ram[0x201] = 0xBE;

        SECTION("bit set")
        {
            chip8.V[0xC] = 0x92;

            chip8.tick();

            REQUIRE(chip8.V[0xF] == 1);
            REQUIRE(chip8.V[0xC] == ((0x92 << 1) & 0xFF));
        }
        SECTION("bit unset")
        {
            chip8.V[0xC] = 0x72;

            chip8.tick();

            REQUIRE(chip8.V[0xF] == 0);
            REQUIRE(chip8.V[0xC] == 0x72 << 1);
        }

        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("SNE Vx Vy")
    {
        chip8.ram[0x200] = 0x90;
        chip8.ram[0x201] = 0xF0;

        chip8.V[0] = 55;

        SECTION("equal")
        {
            chip8.V[15] = 55;

            chip8.tick();

            REQUIRE(chip8.PC == 0x202);
        }
        SECTION("not equal")
        {
            chip8.V[15] = 56;

            chip8.tick();

            REQUIRE(chip8.PC == 0x204);
        }
    }
    SECTION("LD I, nnn")
    {
        chip8.ram[0x200] = 0xAF;
        chip8.ram[0x201] = 0xBC;

        chip8.tick();

        REQUIRE(chip8.I == 0x0FBC);
        REQUIRE(chip8.PC == 0x202);
    }
    SECTION("JP V0, addr")
    {
        chip8.ram[0x200] = 0xB1;
        chip8.ram[0x201] = 0x11;

        SECTION("V0 == 0")
        {
            chip8.tick();

            REQUIRE(chip8.PC == 0x111);
        }
        SECTION("V0 != 0")
        {
            chip8.V[0] = 0xAF;
            chip8.tick();

            REQUIRE(chip8.PC == 0x111 + 0xAF);
        }
    }
    SECTION("RND Vx, byte")
    {
        //Can't really test random, so set mask to 0x00
        chip8.ram[0x200] = 0xCA;
        chip8.ram[0x201] = 0x00;

        chip8.tick();

        REQUIRE(chip8.V[0xA] == 0);
    };
    SECTION("DRW Vx, Vy, nibble")
    {
        //TODO: Write exhaustive tests for rendering @ different positions, test wrapping etc.
        chip8.ram[0x200] = 0xD0;
        chip8.ram[0x201] = 0x01;

        //Put sprite data
        chip8.I = 0x300;
        chip8.ram[0x300] = 0xFF;

        chip8.tick();

        for (unsigned char x = 0; x < 8; ++x)
        {
            REQUIRE(chip8.display[0][x] == 1);
        }
    }
}
