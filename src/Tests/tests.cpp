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
        REQUIRE(chip8.SP == 0);
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
    }
    SECTION("ADD Vx, byte")
    {
        chip8.ram[0x200] = 0x72;
        chip8.ram[0x201] = 0x01;

        chip8.V[0x2] = 0x05;

        chip8.tick();

        REQUIRE(chip8.V[0x2] == 0x05 + 1);
    }
}
