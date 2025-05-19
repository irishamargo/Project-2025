#pragma once
#include <SFML/Graphics.hpp>

class Number
{
public:
    sf::Text text;

    Number();
    Number(const char *buffer, const sf::Font &font, int x, int y);
};

void processKeys(Number** lineNumbers, Number** ColumnNumbers, char* file, int gridSize, sf::Font& font);
void CleanupNumberArray(Number**& array, int gridSize);
