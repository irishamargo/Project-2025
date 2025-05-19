#pragma once
#include "Cell.h"
#include <SFML/Graphics.hpp>

namespace GridUtils
{
    void CreateGrid(Cell **grid, int gridSize);
    void HandleCellClick(Cell **grid, sf::Vector2i mousePos, int gridSize);
    bool Check(const char *name, Cell **grid, int gridSize);
    void ResetGrid(Cell **grid, int gridSize);
}
