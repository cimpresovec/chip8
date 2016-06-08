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
    chip8.loadROM("../roms/PONG");

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
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        chip8.tick();

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
