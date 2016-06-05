#include <iostream>
#include <SFML/Graphics.hpp>
#include "Chip8/Chip8.h"

int main()
{
    //Render window
    sf::RenderWindow window(sf::VideoMode(800, 600), "CHIP8");
    window.setFramerateLimit(60);

    //Chip8
    Chip8 chip8 {};
    chip8.loadROM("../roms/PONG");

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

        //Rendering
        window.clear();
        window.display();
    }

    return 0;
}
