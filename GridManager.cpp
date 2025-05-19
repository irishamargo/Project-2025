#include "GridManager.h"
#include <fstream>
#include <iostream>

const int kCellSize = 45;

void GridUtils::CreateGrid(Cell **grid, int gridSize)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            grid[i][j].shape.setPosition(100 + (55 * j), 100 + (55 * i));
        }
    }
}

void GridUtils::HandleCellClick(Cell **grid, sf::Vector2i mousePos, int gridSize)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (grid[i][j].shape.getGlobalBounds().contains(
                    static_cast<sf::Vector2f>(mousePos)))
            {
                if (grid[i][j].status == 0)
                {
                    grid[i][j].shape.setFillColor(sf::Color{255, 153, 204, 225});
                    grid[i][j].status = 1;
                }
                else if (grid[i][j].status == 1)
                {
                    grid[i][j].shape.setFillColor(sf::Color{255, 255, 255, 225});
                    grid[i][j].status = 2;
                }
                else if (grid[i][j].status == 2)
                {
                    grid[i][j].shape.setFillColor(sf::Color{255, 255, 255, 150});
                    grid[i][j].status = 0;
                }
            }
        }
    }
}

bool GridUtils::Check(const char *name, Cell **grid, int gridSize)
{
    std::ifstream puzzleAnswerFile(name);
    if (!puzzleAnswerFile)
    {
        std::cerr << "File not found!" << std::endl;
        return false;
    }

    std::string line;
    for (int i = 0; i < gridSize; ++i)
    {
        if (!std::getline(puzzleAnswerFile, line))
        {
            return false; // Not enough lines in the answer file
        }
        if (line.length() < static_cast<size_t>(gridSize))
        {
            return false; // Line too short
        }
        for (int j = 0; j < gridSize; ++j)
        {
            char c = line[j];
            if (grid[i][j].status == 1 && c != '1')
            {
                return false;
            }
            else if ((grid[i][j].status == 0 || grid[i][j].status == 2) && c != '0')
            {
                return false;
            }
        }
    }
    return true;
}

void GridUtils::ResetGrid(Cell **grid, int gridSize)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            grid[i][j].status = 0;
            grid[i][j].shape.setFillColor(sf::Color{255, 255, 255, 150});
        }
    }
}
