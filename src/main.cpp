#include <iostream>
#include <SFML/Graphics.hpp>
#include "chip8/chip8.h"

int main()
{
    //Render window
    sf::RenderWindow window(sf::VideoMode(800, 600), "CHIP8");
    window.setFramerateLimit(60);

    //chip8
    Chip8 chip8 {};
    chip8.loadROM("../roms/BRIX");

    //Rendering texture
    sf::Texture display_texture;
    display_texture.create(64, 32);

    //Pixel data
    std::array<sf::Uint8, 64*32*4> pixel_data {};

    //Sprite
    sf::Sprite screen;
    screen.setTexture(display_texture);
    screen.setPosition(0, 0);

    //Viewport
    sf::View view(sf::FloatRect(0, 0, 64, 32));
    window.setView(view);

    while (window.isOpen())
    {
        //Event system
        sf::Event event {};

        while (window.pollEvent(event))
        {
            //Bind input to chip8
            if (event.type == sf::Event::KeyPressed)
            {
                unsigned char val = 1;
                unsigned char key = 0;
                if (event.key.code == sf::Keyboard::C) key = 0;
                else if (event.key.code == sf::Keyboard::Num2) key = 1;
                else if (event.key.code == sf::Keyboard::Num3) key = 2;
                else if (event.key.code == sf::Keyboard::Num4) key = 3;
                else if (event.key.code == sf::Keyboard::W) key = 4;
                else if (event.key.code == sf::Keyboard::E) key = 5;
                else if (event.key.code == sf::Keyboard::R) key = 6;
                else if (event.key.code == sf::Keyboard::S) key = 7;
                else if (event.key.code == sf::Keyboard::D) key = 8;
                else if (event.key.code == sf::Keyboard::F) key = 9;
                else if (event.key.code == sf::Keyboard::X) key = 10;
                else if (event.key.code == sf::Keyboard::V) key = 11;
                else if (event.key.code == sf::Keyboard::Num5) key = 12;
                else if (event.key.code == sf::Keyboard::T) key = 13;
                else if (event.key.code == sf::Keyboard::G) key = 14;
                else if (event.key.code == sf::Keyboard::B) key = 15;

                //Set chip data
                if (chip8.need_new)
                {
                    chip8.new_press = true;
                    chip8.last_pressed = key;
                }
                chip8.input[key] = val;
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                unsigned char val = 0;
                if (event.key.code == sf::Keyboard::C) chip8.input[0] = val;
                else if (event.key.code == sf::Keyboard::Num2) chip8.input[1] = val;
                else if (event.key.code == sf::Keyboard::Num3) chip8.input[2] = val;
                else if (event.key.code == sf::Keyboard::Num4) chip8.input[3] = val;
                else if (event.key.code == sf::Keyboard::W) chip8.input[4] = val;
                else if (event.key.code == sf::Keyboard::E) chip8.input[5] = val;
                else if (event.key.code == sf::Keyboard::R) chip8.input[6] = val;
                else if (event.key.code == sf::Keyboard::S) chip8.input[7] = val;
                else if (event.key.code == sf::Keyboard::D) chip8.input[8] = val;
                else if (event.key.code == sf::Keyboard::F) chip8.input[9] = val;
                else if (event.key.code == sf::Keyboard::X) chip8.input[10] = val;
                else if (event.key.code == sf::Keyboard::V) chip8.input[11] = val;
                else if (event.key.code == sf::Keyboard::Num5) chip8.input[12] = val;
                else if (event.key.code == sf::Keyboard::T) chip8.input[13] = val;
                else if (event.key.code == sf::Keyboard::G) chip8.input[14] = val;
                else if (event.key.code == sf::Keyboard::B) chip8.input[15] = val;
            }

            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        //Fixed 600Hz
        for (int i = 0; i < 10; ++i)
        {
            chip8.tick();
        }

        chip8.tickTimers();

        //TODO: TEMP UPDATE TEXTURE
        unsigned int pos = 0;
        for (unsigned int y = 0; y < 32; ++y)
        {
            for (unsigned int x = 0; x < 64; ++x)
            {
                if (chip8.display[y][x])
                {
                    pixel_data[pos] = 255;
                    pixel_data[pos + 1] = 255;
                    pixel_data[pos + 2] = 255;
                    pixel_data[pos + 3] = 255;
                }
                else
                {
                    pixel_data[pos] = 0;
                    pixel_data[pos + 1] = 0;
                    pixel_data[pos + 2] = 0;
                    pixel_data[pos + 3] = 255;
                }
                pos += 4;
            }
        }
        display_texture.update(pixel_data.data());

        //Rendering
        window.clear();

        window.draw(screen);

        window.display();
    }

    return 0;
}
