#include "workingKeys.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

// Реализация методов класса Number
Number::Number()
{
    text.setString(' ');
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

void processKeys(Number** lineNumbers, Number** ColumnNumbers, char* file, int gridSize, sf::Font& font) {
    std::ifstream puzzleFile(file);
    if (!puzzleFile)
    {
        std::cerr << "File not found!" << std::endl;
        return;
    }

        // Для строк
    int countColumns{};
    int countLines = 5;
    char* sizeBuffer = new char[gridSize + 1];
    puzzleFile.getline(sizeBuffer, gridSize);

    int numberX = 80;
    int numberY = 110;
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j) {
            lineNumbers[i][j] = Number();
        }

        char buffer[gridSize*2];
        puzzleFile.getline(buffer, sizeof(buffer));

        int p = 0;
        int current_pos = 0;

        for (int j = 0; buffer[j] != '\0' && p < gridSize; ++j) {
            if (buffer[j] == ' ') {
                if (current_pos > 0) {
                    char num_str[32] = {0};
                    strncpy(num_str, &buffer[j - current_pos], current_pos);
                    lineNumbers[i][p] = Number(num_str, font, numberX - (p * 35), numberY);
                    p++;
                    current_pos = 0;
                }
            } else {
            current_pos++;
            }
        }

        if (current_pos > 0 && p < gridSize) {
            char num_str[32] = {0};
            strncpy(num_str, &buffer[strlen(buffer) - current_pos], current_pos);
            lineNumbers[i][p] = Number(num_str, font, numberX - (p * 35), numberY);
        }
        numberY += 55;
    }

    // Для столбцов
    numberX = 120;
    numberY = 65;
    for (int i = 0; i < gridSize; ++i)
    {
        char buffer[gridSize*2];
        puzzleFile.getline(buffer, sizeof(buffer));

        int p = 0;
        int current_pos = 0;
        for (int j = 0; j < gridSize; ++j) {
            ColumnNumbers[i][j] = Number();
        }

        for (int j = 0; buffer[j] != '\0' && p < gridSize; ++j) {
            if (buffer[j] == ' ') {
                if (current_pos > 0) {
                    char num_str[32] = {0};
                    strncpy(num_str, &buffer[j - current_pos], current_pos);
                    ColumnNumbers[i][p] = Number(num_str, font, numberX, numberY - (p * 30));
                    p++;
                    current_pos = 0;
                }
            } else {
                current_pos++;
            }
        }

        if (current_pos > 0 && p < gridSize) {
            char num_str[32] = {0};
            strncpy(num_str, &buffer[strlen(buffer) - current_pos], current_pos);
            ColumnNumbers[i][p] = Number(num_str, font, numberX, numberY - (p * 30));
        }
        numberX += 55;
    }

    delete[] sizeBuffer;
}

void CleanupNumberArray(Number**& array, int gridSize) {
    if (array) {
        for (int i = 0; i < gridSize; ++i) {
            delete[] array[i];
        }
        delete[] array;
        array = nullptr;
    }
}
