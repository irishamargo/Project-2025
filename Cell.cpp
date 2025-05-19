#include "Cell.h"

Cell::Cell()
{
    shape.setSize(sf::Vector2f(kCellSize, kCellSize));
    shape.setFillColor(sf::Color{255, 255, 255, 150});
    shape.setPosition(0, 0);
    status = 0;
}

Cell::Cell(int x, int y)
{
    shape.setSize(sf::Vector2f(kCellSize, kCellSize));
    shape.setFillColor(sf::Color{255, 255, 255, 150});
    shape.setPosition(x, y);
    status = 0;
}
