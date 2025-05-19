#include "Number.h"
#include <SFML/Graphics.hpp>

Number::Number()
{
    text.setString(" ");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(0, 0);
}

Number::Number(const char *buffer, const sf::Font &font, int x, int y)
{
    text.setString(buffer);
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}
