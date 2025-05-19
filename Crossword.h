#ifndef CROSSWORD_H
#define CROSSWORD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cstring>

namespace
{
    const int kCellSize = 50;
    const int kGridSize = 15;
    const int kMaxPuzzles = 3;
}

class Cell
{
public:
    sf::RectangleShape shape;
    int status;

    Cell();
    Cell(int x, int y);
};

void CreateGrid(Cell **grid, int gridSize);
void HandleCellClick(Cell **grid, sf::Vector2i mousePos, int gridSize);

#endif // CROSSWORD_H
