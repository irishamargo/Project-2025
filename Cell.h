#pragma once
#include <SFML/Graphics.hpp>

class Cell
{
public:
    Cell();
    Cell(int x, int y);

    sf::RectangleShape shape;
    int status;

    static constexpr float kCellSize = 50.0f;
};
